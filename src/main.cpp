// #Generation-Assisted-With-GPT-6#
// #Generation-Assisted-with-OpenAI#
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include "developer_scripts.hpp"

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <cstring>
#else
#include <unistd.h>
#endif

namespace fs = std::filesystem;
namespace {
constexpr const char* release_version = "0002";
// References the project's foundational CORRECTIONS.md record. Not a security certification.
constexpr const char* security_record = "0000-0000-0000-0001";

bool output_is_terminal() {
#ifdef _WIN32
    return _isatty(_fileno(stdout)) != 0;
#else
    return isatty(STDOUT_FILENO) != 0;
#endif
}

bool enable_color() {
    if (std::getenv("NO_COLOR") || !output_is_terminal()) return false;
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h == INVALID_HANDLE_VALUE) return false;
    DWORD mode = 0;
    if (!GetConsoleMode(h, &mode)) return false;
    return SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
#else
    return true;
#endif
}

void banner(bool color) {
    std::cout << "Greetings, and Welcome to AI-Device-Bridge.\n";
    if (color) std::cout << "\033[1;35m";
    std::cout << "[ Pre-Alpha-Build ]";
    if (color) std::cout << "\033[0m";
    std::cout << "  Release " << release_version << '\n';
}

struct Device {
    std::string name;
    std::string location;
    std::string driver;
    bool functioning = false; // Windows: PnP reports a started node, not inference health.
};

#ifdef _WIN32
std::string lowercase_ascii(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c){return static_cast<char>(std::tolower(c));});
    return value;
}
#endif

std::vector<Device> discover_devices() {
    std::vector<Device> devices;
#ifdef __linux__
    // Linux's PCI sysfs is read-only here; device discovery never binds drivers.
    const fs::path pci_devices{"/sys/bus/pci/devices"};
    std::error_code ec;
    fs::directory_iterator it(pci_devices, fs::directory_options::skip_permission_denied, ec), end;
    while (!ec && it != end) {
        const auto& entry = *it;
        std::ifstream vendor_file(entry.path() / "vendor");
        std::string vendor;
        if (vendor_file >> vendor) {
            std::transform(vendor.begin(), vendor.end(), vendor.begin(), [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
            if (vendor == "0x1e60") {
                std::ifstream device_file(entry.path() / "device");
                std::string device_id;
                device_file >> device_id;
                std::transform(device_id.begin(), device_id.end(), device_id.begin(),
                               [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
                const bool hailo8 = device_id == "0x2864";
                devices.push_back({hailo8 ? "Hailo-8 PCIe accelerator (M.2 form factor unverified)" :
                                            "Hailo PCIe accelerator (model unverified)",
                                   entry.path().filename().string() + " [1e60:" +
                                   (device_id.empty() ? "unknown" : device_id.substr(2)) + "]", "", false});
            }
        }
        it.increment(ec);
    }
#elif defined(_WIN32)
    // SetupAPI enumerates present PnP devices without requiring administrator rights.
    // This checks only the initial Hailo vendor family; other AI devices need adapters.
    HDEVINFO inventory = SetupDiGetClassDevsA(nullptr, nullptr, nullptr, DIGCF_PRESENT | DIGCF_ALLCLASSES);
    if (inventory != INVALID_HANDLE_VALUE) {
        for (DWORD index = 0;; ++index) {
            SP_DEVINFO_DATA data{};
            data.cbSize = sizeof(data);
            if (!SetupDiEnumDeviceInfo(inventory, index, &data)) break;
            char ids[4096]{};
            if (!SetupDiGetDeviceRegistryPropertyA(inventory, &data, SPDRP_HARDWAREID,
                    nullptr, reinterpret_cast<PBYTE>(ids), sizeof(ids), nullptr)) continue;
            const std::string hardware = lowercase_ascii(ids);
            if (hardware.find("ven_1e60") == std::string::npos) continue;
            const bool hailo8 = hardware.find("dev_2864") != std::string::npos;
            char service[512]{};
            char description[1024]{};
            SetupDiGetDeviceRegistryPropertyA(inventory, &data, SPDRP_SERVICE, nullptr,
                                             reinterpret_cast<PBYTE>(service), sizeof(service), nullptr);
            SetupDiGetDeviceRegistryPropertyA(inventory, &data, SPDRP_DEVICEDESC, nullptr,
                                             reinterpret_cast<PBYTE>(description), sizeof(description), nullptr);
            ULONG status = 0, problem = 0;
            const bool started = CM_Get_DevNode_Status(&status, &problem, data.DevInst, 0) == CR_SUCCESS &&
                                 (status & DN_STARTED) != 0 && problem == 0;
            const std::string model = hailo8 ? "Hailo-8 PCIe accelerator (M.2 form factor unverified)" :
                                              "Hailo PCIe accelerator (model unverified)";
            const std::string id = hailo8 ? "1e60:2864" : "1e60:unknown";
            devices.push_back({model, std::string(description[0] ? description : "PnP device") + " [" + id + "]",
                               service, started});
        }
        SetupDiDestroyDeviceInfoList(inventory);
    }
#endif
    return devices;
}

bool executable_on_path(const std::string& name) {
    const char* env = std::getenv("PATH");
    if (!env) return false;
#ifdef _WIN32
    constexpr char separator = ';';
    const std::vector<std::string> candidates{name + ".exe", name};
#else
    constexpr char separator = ':';
    const std::vector<std::string> candidates{name};
#endif
    const std::string path(env);
    size_t from = 0;
    while (from <= path.size()) {
        size_t until = path.find(separator, from);
        auto segment = path.substr(from, until == std::string::npos ? std::string::npos : until - from);
        fs::path directory = segment.empty() ? fs::path(".") : fs::path(segment);
        for (const auto& candidate : candidates) {
            std::error_code ec;
            if (fs::is_regular_file(directory / candidate, ec)) {
#ifdef _WIN32
                return true;
#else
                if (access((directory / candidate).c_str(), X_OK) == 0) return true;
#endif
            }
        }
        if (until == std::string::npos) break;
        from = until + 1;
    }
    return false;
}

void print_devices() {
    const auto devices = discover_devices();
    std::cout << "AI device discovery (preliminary, read-only):\n";
    if (devices.empty()) {
        std::cout << "  No supported PCIe accelerators detected by this prototype.\n";
        std::cout << "  This does not establish that no AI hardware is installed.\n";
    }
    for (const auto& device : devices)
        std::cout << "  - " << device.name << " at " << device.location << '\n';
    std::cout << "  Backend inference support: not implemented yet.\n";
}

enum class TestStatus { Pass, Missing, Warning, Untested };

void report_check(TestStatus status, const std::string& label, const std::string& detail) {
    const char* tag = "UNKNOWN";
    switch (status) {
        case TestStatus::Pass: tag = "PASS"; break;
        case TestStatus::Missing: tag = "MISSING"; break;
        case TestStatus::Warning: tag = "WARNING"; break;
        case TestStatus::Untested: tag = "UNTESTED"; break;
    }
    std::cout << "  [" << tag << "] " << label << ": " << detail << '\n';
}

void diagnose_system() {
    std::cout << "\nSystem diagnostics — packages and prerequisites (read-only)\n";
#ifdef __linux__
    report_check(TestStatus::Pass, "Host OS", "Linux");
    std::error_code ec;
    report_check(fs::is_directory("/sys/bus/pci/devices", ec) ? TestStatus::Pass : TestStatus::Warning,
                 "PCI device inventory", "Linux PCI sysfs accessibility");
#elif defined(_WIN32)
    report_check(TestStatus::Pass, "Host OS", "Windows");
    report_check(TestStatus::Pass, "Device inventory", "Windows SetupAPI/PnP enumeration available (read-only)");
#else
    report_check(TestStatus::Pass, "Host OS", "Other platform");
    report_check(TestStatus::Untested, "Device inventory", "Hardware discovery not implemented");
#endif
    // Build tools are optional at runtime; absence is not a runtime failure.
    report_check(executable_on_path("cmake") ? TestStatus::Pass : TestStatus::Warning,
                 "CMake", executable_on_path("cmake") ? "available (version unverified)" : "not on PATH; needed only to build from source");
    const bool compiler = executable_on_path("c++") || executable_on_path("g++") || executable_on_path("clang++") || executable_on_path("cl");
#ifdef _WIN32
    const bool winget = executable_on_path("winget");
    report_check(winget ? TestStatus::Pass : TestStatus::Warning, "winget",
                 winget ? "available for optional user-managed installation" : "not found on PATH; optional build setup tool");
    const bool mingw = executable_on_path("g++") || executable_on_path("x86_64-w64-mingw32-g++");
    report_check(mingw ? TestStatus::Pass : TestStatus::Warning, "MinGW toolchain",
                 mingw ? "compiler found (architecture and C++20 support unverified)" : "not on PATH; optionally install MSYS2/UCRT64 MinGW-w64");
    report_check(executable_on_path("subl") ? TestStatus::Pass : TestStatus::Warning, "Sublime Text CLI",
                 executable_on_path("subl") ? "subl command found" : "not found on PATH; editor is optional");
#endif
    report_check(compiler ? TestStatus::Pass : TestStatus::Warning,
                 "C++ compiler", compiler ? "found on PATH (C++20 support unverified)" : "not on PATH; needed only to build from source");
    const bool hailort = executable_on_path("hailortcli");
    report_check(hailort ? TestStatus::Pass : TestStatus::Missing,
                 "HailoRT CLI", hailort ? "executable found on PATH (operation/version unverified)" : "hailortcli not found on PATH; needed for Hailo utilities");
    report_check(TestStatus::Untested, "HailoRT shared library", "not checked by this pre-alpha build");
    std::cout << "  No installation, package changes, or privileged commands performed.\n";
}

void diagnose_device() {
    std::cout << "\nDevice diagnostics — hardware functionality (read-only)\n";
    const auto devices = discover_devices();
#ifdef __linux__
    if (devices.empty()) {
        report_check(TestStatus::Missing, "Hailo PCIe device", "no Hailo vendor ID (0x1e60) detected in PCI sysfs");
    } else {
        for (const auto& device : devices)
            report_check(TestStatus::Pass, "PCIe enumeration", device.name + " at " + device.location);
    }
    std::error_code ec;
    const bool node = fs::exists("/dev/hailo0", ec);
    report_check(node ? TestStatus::Pass : TestStatus::Missing,
                 "Hailo device node", node ? "/dev/hailo0 exists (access not verified)" : "/dev/hailo0 not found");
    if (node) {
        const bool access_ok = access("/dev/hailo0", R_OK | W_OK) == 0;
        report_check(access_ok ? TestStatus::Pass : TestStatus::Warning,
                     "Device node permissions", access_ok ? "read/write access available" : "read/write access denied or unavailable");
    }
#elif defined(_WIN32)
    if (devices.empty()) report_check(TestStatus::Missing, "Hailo PCIe device", "no present device with PCI vendor 1e60 found through Windows SetupAPI");
    for (const auto& device : devices) {
        report_check(TestStatus::Pass, "PnP enumeration", device.name + " at " + device.location);
        report_check(device.functioning ? TestStatus::Pass : TestStatus::Warning,
                     "PnP device state", device.functioning ? "device node started (not inference-verified)" : "device node not reported started; inspect Device Manager");
    }
#else
    report_check(TestStatus::Untested, "Hardware enumeration", "native device enumeration not implemented for this OS");
#endif
    report_check(TestStatus::Untested, "Firmware health / inference", "requires a future HailoRT-backed active test; no device commands were sent");
    std::cout << "  Presence does not prove device health or inference functionality.\n";
}

void diagnose_driver() {
    std::cout << "\nDriver diagnostics — binding and runtime validation (read-only)\n";
#ifdef __linux__
    const auto devices = discover_devices();
    if (devices.empty()) {
        report_check(TestStatus::Missing, "Hailo PCIe binding", "no Hailo PCIe device available to inspect");
    }
    for (const auto& device : devices) {
        const fs::path location = fs::path("/sys/bus/pci/devices") / device.location;
        std::error_code ec;
        const fs::path binding = fs::read_symlink(location / "driver", ec);
        if (!ec) {
            report_check(TestStatus::Pass, "Kernel driver binding " + device.location,
                         "bound to " + binding.filename().string() + " (function not validated)");
        } else {
            report_check(TestStatus::Missing, "Kernel driver binding " + device.location,
                         "no readable driver binding detected");
        }
    }
    const fs::path module{ "/sys/module/hailo_pci" };
    std::error_code ec;
    report_check(fs::exists(module, ec) ? TestStatus::Pass : TestStatus::Warning,
                 "hailo_pci kernel module", fs::exists(module, ec) ? "module sysfs entry present" : "module sysfs entry not found");
#elif defined(_WIN32)
    const auto devices = discover_devices();
    if (devices.empty()) report_check(TestStatus::Missing, "Hailo PnP driver", "no present Hailo PCIe device found");
    for (const auto& device : devices) {
        report_check(device.driver.empty() ? TestStatus::Missing : TestStatus::Pass,
                     "PnP driver service", device.driver.empty() ? "no driver service reported for " + device.location : device.driver + " for " + device.location);
        report_check(device.functioning ? TestStatus::Pass : TestStatus::Warning, "PnP started state",
                     device.functioning ? "device started (runtime functionality unverified)" : "device not reported started; check Device Manager error code");
    }
#else
    report_check(TestStatus::Untested, "Kernel driver binding", "not implemented for this OS");
#endif
    const bool cli = executable_on_path("hailortcli");
    report_check(cli ? TestStatus::Pass : TestStatus::Missing, "HailoRT CLI",
                 cli ? "present; version and driver compatibility not yet verified" : "not found on PATH");
    report_check(TestStatus::Untested, "Driver/runtime compatibility", "requires version query and actual runtime health check in a later build");
    std::cout << "  No driver loaded, unloaded, installed, or modified.\n";
}

void print_diagnostics() {
    std::cout << "AI Device Bridge diagnostics — summary\n";
    diagnose_system();
    diagnose_device();
    diagnose_driver();
}

void diagnostics_menu() {
    for (;;) {
        std::cout << "\nDiagnostics\n"
                  << "1) System — packages and prerequisites\n"
                  << "2) Device — detection and functionality\n"
                  << "3) Driver — binding and runtime validation\n"
                  << "4) Run all diagnostics\n"
                  << "0) Return to main menu\nSelection: " << std::flush;
        std::string choice;
        if (!std::getline(std::cin, choice) || choice == "0") return;
        if (choice == "1") diagnose_system();
        else if (choice == "2") diagnose_device();
        else if (choice == "3") diagnose_driver();
        else if (choice == "4") print_diagnostics();
        else std::cout << "Unknown selection.\n";
    }
}

fs::path model_search_directory() {
    const char* root_env = std::getenv("AIDB_MODEL_DIR");
    return (root_env && *root_env) ? fs::path(root_env) : fs::path("models");
}

std::string lowercase(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return s;
}

struct FileType {
    const char* extension;
    const char* description;
};

constexpr FileType file_types[] = {
    {".hef", "Hailo executable"},
    {".onnx", "ONNX model"},
    {".gguf", "GGUF model"},
    {".safetensors", "Safetensors model"},
    {".tflite", "TensorFlow Lite model"},
};

std::vector<fs::path> find_files(const fs::path& root, const std::string& extension) {
    std::vector<fs::path> files;
    std::error_code ec;
    if (!fs::is_directory(root, ec)) return files;
    fs::directory_iterator it(root, fs::directory_options::skip_permission_denied, ec), end;
    while (!ec && it != end) {
        const auto& entry = *it;
        std::error_code entry_ec;
        if (entry.is_regular_file(entry_ec)) {
            const std::string ext = lowercase(entry.path().extension().string());
            bool matches = extension == "*";
            if (!matches) matches = ext == extension;
            else {
                matches = false;
                for (const auto& type : file_types)
                    if (ext == type.extension) { matches = true; break; }
            }
            if (matches) files.push_back(entry.path());
        }
        it.increment(ec);
    }
    std::sort(files.begin(), files.end());
    return files;
}

void print_models() {
    const auto root = model_search_directory();
    std::cout << "HEF model discovery (file names only):\n";
    std::error_code ec;
    if (!fs::is_directory(root, ec)) {
        std::cout << "  Directory not found: " << root.string() << '\n';
        std::cout << "  Set AIDB_MODEL_DIR to an existing local directory.\n";
        return;
    }
    const auto files = find_files(root, ".hef");
    for (const auto& file : files)
        std::cout << "  - " << file.filename().string() << '\n';
    if (files.empty()) std::cout << "  No .hef files found in " << root.string() << '\n';
    std::cout << "  Files are not validated, loaded or executed.\n";
}

void file_selection_menu() {
    fs::path root = model_search_directory();
    for (;;) {
        std::cout << "\nFind / Select Files\n"
                  << "Search directory: " << root.string() << "\n"
                  << "1) *.hef          Hailo executable\n"
                  << "2) *.onnx         ONNX models\n"
                  << "3) *.gguf         GGUF models\n"
                  << "4) *.safetensors  Safetensors models\n"
                  << "5) *.tflite       TensorFlow Lite models\n"
                  << "6) All supported model files\n"
                  << "7) Change search directory\n"
                  << "0) Return to main menu\nSelection: " << std::flush;
        std::string choice;
        if (!std::getline(std::cin, choice) || choice == "0") return;
        if (choice == "7") {
            std::cout << "Enter directory path (blank cancels): " << std::flush;
            std::string path;
            if (!std::getline(std::cin, path)) return;
            if (!path.empty()) {
                std::error_code ec;
                if (fs::is_directory(fs::path(path), ec)) root = fs::path(path);
                else std::cout << "Directory not found; search directory unchanged.\n";
            }
            continue;
        }
        const std::string options = "123456";
        if (choice.size() != 1 || options.find(choice[0]) == std::string::npos) {
            std::cout << "Unknown selection.\n";
            continue;
        }
        const std::string ext = choice == "6" ? "*" : file_types[static_cast<size_t>(choice[0] - '1')].extension;
        const auto files = find_files(root, ext);
        std::cout << "\nMatching files (not validated or loaded):\n";
        if (files.empty()) {
            std::cout << "  No matching files found.\n";
            continue;
        }
        for (size_t i = 0; i < files.size(); ++i)
            std::cout << (i + 1) << ") " << files[i].filename().string() << '\n';
        std::cout << "0) Back to file types\nSelect file number: " << std::flush;
        std::string number;
        if (!std::getline(std::cin, number)) return;
        if (number == "0") continue;
        try {
            size_t consumed = 0;
            const unsigned long long index = std::stoull(number, &consumed);
            if (consumed != number.size() || index == 0 || index > files.size())
                throw std::out_of_range("selection");
            std::cout << "Selected file: " << fs::absolute(files[static_cast<size_t>(index - 1)]).string() << '\n';
            std::cout << "Selection is informational; no model was loaded or executed.\n";
        } catch (const std::exception&) {
            std::cout << "Invalid file selection.\n";
        }
    }
}

void hailo8_links() {
    std::cout << "\nHailo-8 M.2 — Device Resources (URLs for manual use)\n"
              << "  Hailo official site: https://hailo.ai/\n"
              << "  Hailo Developer Zone / driver downloads: https://hailo.ai/developer-zone/\n"
              << "  HailoRT for Hailo-8: https://github.com/hailo-ai/hailort/tree/hailo8\n"
              << "  Hailo Model Zoo (Hailo-8 v2.x): https://github.com/hailo-ai/hailo_model_zoo/tree/v2.x\n"
              << "  Hailo Community: https://community.hailo.ai/\n"
              << "  Driver and model downloads may require an official Hailo account.\n"
              << "  Detection identifies a PCIe device, not verified firmware/inference health.\n";
}

// The CLI report is noninteractive, and includes discovery status even when
// no supported devices are present. It never claims that absent hardware is connected.
void website_links_report() {
    std::cout << "\nWebsite Links — Detected AI Devices (read-only)\n"
              << "  Project: https://github.com/DGApproved/AI-Device-Bridge\n";
    const auto devices = discover_devices();
    bool supported = false;
    for (const auto& device : devices) {
        if (device.location.find("[1e60:2864]") != std::string::npos) {
            supported = true;
            std::cout << "  1) Hailo 8.0 M.2 [detected at " << device.location
                      << "; physical M.2 form factor unverified]\n";
        } else {
            std::cout << "  Detected (links not yet registered): " << device.name
                      << " at " << device.location << '\n';
        }
    }
    if (supported) hailo8_links();
    else std::cout << "  No supported Hailo-8 devices detected; device-specific links are not listed.\n"
                   << "  See diagnostics and official manufacturer documentation for setup.\n";
}

void website_links_menu() {
    for (;;) {
        std::cout << "\nWebsite Links — Detected AI Devices\n"
                  << "  Project: https://github.com/DGApproved/AI-Device-Bridge\n";
        const auto devices = discover_devices();
        bool hailo8_found = false;
        for (const auto& device : devices) {
            if (device.location.find("[1e60:2864]") != std::string::npos) {
                hailo8_found = true;
                std::cout << "1) Hailo 8.0 M.2 — " << device.location
                          << " (form factor unverified)\n";
            } else {
                std::cout << "  Device without links profile: " << device.name
                          << " at " << device.location << '\n';
            }
        }
        if (!hailo8_found) std::cout << "  No supported AI devices detected for device-specific links.\n";
        std::cout << "0) Return to Help\nSelection: " << std::flush;
        std::string choice;
        if (!std::getline(std::cin, choice) || choice == "0") return;
        if (choice == "1" && hailo8_found) hailo8_links();
        else std::cout << "Selection unavailable: discover a supported device first.\n";
    }
}

bool debian_family() {
#ifdef __linux__
    std::ifstream os("/etc/os-release");
    std::string line;
    while (std::getline(os, line)) {
        if (line.rfind("ID=", 0) == 0 || line.rfind("ID_LIKE=", 0) == 0) {
            if (line.find("debian") != std::string::npos || line.find("ubuntu") != std::string::npos) return true;
        }
    }
#endif
    return false;
}

void repair_instructions() {
    std::cout << "\nRepair / Installation Commands (DISPLAY ONLY; no commands executed)\n"
              << "These suggestions are based on the checks currently available to Diagnostics.\n"
              << "Check versions and official vendor instructions BEFORE modifying a working installation.\n\n";
#ifdef __linux__
    const bool apt = debian_family();
    const bool cmake = executable_on_path("cmake");
    const bool compiler = executable_on_path("c++") || executable_on_path("g++") || executable_on_path("clang++");
    const bool cli = executable_on_path("hailortcli");
    const bool devices = !discover_devices().empty();
    std::error_code ec;
    const bool node = fs::exists("/dev/hailo0", ec);
    const bool module = fs::exists("/sys/module/hailo_pci", ec);
    std::cout << "Step 1 — Examine current system (read-only):\n"
              << "  cat /etc/os-release\n"
              << "  uname -r\n"
              << "  lspci -nn | grep -i hailo\n"
              << "  ls -l /dev/hailo*\n"
              << "  lsmod | grep hailo\n"
              << "  command -v hailortcli\n"
              << "  hailortcli --version\n";
    if (!cmake || !compiler) {
        std::cout << "\nStep 2 — Optional tools for building from source (" << (!cmake ? "CMake missing; " : "")
                  << (!compiler ? "compiler missing" : "") << "):\n";
        if (apt) std::cout << "  sudo apt update\n  sudo apt install build-essential cmake\n";
        else std::cout << "  Install a C++20 compiler and CMake using your distribution's package manager.\n";
        std::cout << "  (Build tools are NOT required just to run a prebuilt executable.)\n";
    } else {
        std::cout << "\nStep 2 — Build tools detected; no installation suggested.\n";
    }
    if (!devices) {
        std::cout << "\nStep 3 — Hailo PCIe hardware not detected by preliminary discovery:\n"
                  << "  lspci -nn -d 1e60:\n"
                  << "  Inspect physical installation and BIOS/PCIe visibility.\n"
                  << "  Do not install drivers solely to fix a PCIe device that is not enumerating.\n";
    } else {
        std::cout << "\nStep 3 — Hailo PCIe vendor detected; confirm exact device model:\n"
                  << "  lspci -nn -d 1e60:\n";
    }
    if (!module || !node || !cli) {
        std::cout << "\nStep 4 — Driver/runtime setup guidance:\n";
        if (!module) std::cout << "  Kernel module not detected; inspect: modinfo hailo_pci\n";
        if (!node) std::cout << "  Device node absent; confirm driver binding before changing permissions.\n";
        if (!cli) std::cout << "  HailoRT CLI missing from PATH or not installed.\n";
        std::cout << "  Download the compatible Hailo-8 driver and HailoRT from:\n"
                  << "  https://hailo.ai/developer-zone/\n"
                  << "  Review the official installation instructions and package requirements.\n";
        if (apt) std::cout << "  Example for a verified official local .deb package:\n"
                           << "    sudo apt install ./<official-package-filename>.deb\n"
                           << "  Replace the placeholder ONLY after confirming its package type and compatibility.\n";
        std::cout << "  Do not blindly mix driver and runtime versions or overwrite working installations.\n";
    } else std::cout << "\nStep 4 — Module, device node and HailoRT CLI detected; no reinstall suggested.\n";
    std::cout << "\nStep 5 — Verify after any MANUAL changes (read-only probes):\n"
              << "  hailortcli --version\n"
              << "  hailortcli fw-control identify\n"
              << "  aidevicebridge --diagnose\n"
              << "  Firmware identification does not replace a validated inference test.\n";
#elif defined(_WIN32)
    const auto devices = discover_devices();
    const bool cmake_present = executable_on_path("cmake");
    const bool mingw_present = executable_on_path("g++") || executable_on_path("x86_64-w64-mingw32-g++");
    const bool winget_present = executable_on_path("winget");
    std::cout << "Step 1 — Inspect currently present devices (PowerShell; manual commands):\n"
              << "  Get-PnpDevice -PresentOnly | Where-Object { $_.InstanceId -match 'VEN_1E60' }\n"
              << "  Get-Command hailortcli -ErrorAction SilentlyContinue\n"
              << "  where.exe hailortcli\n";
    std::cout << "\nStep 2 — Build environment (optional when running a prebuilt executable):\n";
    if (!winget_present) std::cout << "  winget not found; install/update Microsoft App Installer from an authorized source.\n";
    if (!cmake_present) std::cout << "  winget search --name CMake\n  winget install --id Kitware.CMake --exact\n";
    else std::cout << "  CMake detected; no reinstall recommended.\n";
    if (!mingw_present) std::cout << "  winget search --name MSYS2\n  winget install --id MSYS2.MSYS2 --exact\n"
                                  << "  # In the MSYS2 UCRT64 terminal, manually run:\n"
                                  << "  pacman -Syu\n  pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja\n";
    else std::cout << "  MinGW compiler detected; verify its architecture and C++20 support.\n";
    if (!executable_on_path("subl")) std::cout << "  winget search --name Sublime\n  winget install --id SublimeHQ.SublimeText.4 --exact\n"
                                            << "  # Sublime Text is optional and may not add 'subl' to PATH.\n";
    std::cout << "  # From MSYS2 UCRT64, in the repository directory:\n"
              << "  cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release\n"
              << "  cmake --build build\n"
              << "  ctest --test-dir build --output-on-failure\n";
    if (devices.empty()) std::cout << "\nStep 3 — No present Hailo PCIe device found. Check Device Manager and hardware installation.\n";
    else {
        std::cout << "\nStep 3 — Hailo PCIe device(s) present; inspect driver service and Device Manager status:\n";
        for (const auto& d : devices) std::cout << "  " << d.location << " | service: " << (d.driver.empty() ? "not reported" : d.driver) << "\n";
    }
    if (!executable_on_path("hailortcli")) std::cout << "  HailoRT CLI was not found in PATH (may be installed elsewhere).\n";
    std::cout << "\nStep 4 — Consult the official Hailo site to verify WINDOWS support for your exact device, driver, and runtime release:\n"
              << "  https://hailo.ai/developer-zone/\n"
              << "  Do not use Linux .deb files, Linux driver commands, or unverified third-party drivers on Windows.\n"
              << "Step 5 — After any manual installation, re-run:\n"
              << "  aidevicebridge.exe --diagnose\n"
              << "  aidevicebridge.exe --devices\n"
              << "PnP enumeration is not an inference or firmware health test.\n";
#else
    std::cout << "Automated repair guidance is not implemented for this operating system.\n"
              << "Consult your accelerator manufacturer's driver/runtime installation instructions.\n";
#endif
    std::cout << "\nNo commands have been executed, and no system settings have changed.\n";
}

void help() {
    std::cout << "Options: --help --links --repair-guide --version --security-version --devices --diagnose --diagnose-system --diagnose-device --diagnose-driver --models --developer-menu --export-dev-setup --menu\n";
    std::cout << "Model search: set AIDB_MODEL_DIR to a directory (Windows example: set AIDB_MODEL_DIR=C:\\models)\n";
    std::cout << "Pre-alpha: listing model files does not validate or execute them.\n";
    std::cout << "Use menu option 4 for website links, repair guidance, and these instructions.\n";
}

void help_menu() {
    for (;;) {
        std::cout << "\nHelp\n"
                  << "1) Website Links\n"
                  << "2) Repair / Installation Commands\n"
                  << "3) General Help / Command-Line Options\n"
                  << "0) Return to main menu\nSelection: " << std::flush;
        std::string choice;
        if (!std::getline(std::cin, choice) || choice == "0") return;
        if (choice == "1") website_links_menu();
        else if (choice == "2") repair_instructions();
        else if (choice == "3") help();
        else std::cout << "Unknown selection.\n";
    }
}


std::string developer_script_name() {
#ifdef _WIN32
    return "setup-cpp-toolchain.bat";
#elif defined(__linux__)
    return "setup-cpp-toolchain-linux.sh";
#else
    return "";
#endif
}

const char* developer_script_contents() {
#ifdef _WIN32
    return aidb_scripts::windows;
#elif defined(__linux__)
    return aidb_scripts::linux_script;
#else
    return nullptr;
#endif
}

fs::path developer_downloads_dir() {
#ifdef _WIN32
    if (const char* user = std::getenv("USERPROFILE"); user && *user) return fs::path(user) / "Downloads";
    return {};
#else
    if (const char* home = std::getenv("HOME"); home && *home) return fs::path(home) / "Downloads";
    return {};
#endif
}

bool export_developer_script(const fs::path& root, std::ostream& out) {
    const char* contents = developer_script_contents();
    const std::string name = developer_script_name();
    if (!contents || name.empty()) {
        out << "No developer setup script is available for this operating system.\n";
        return false;
    }
    if (root.empty()) { out << "Cannot determine your Downloads directory.\n"; return false; }
    std::error_code ec;
    fs::create_directories(root, ec);
    if (ec || !fs::is_directory(root)) { out << "Cannot create or access Downloads: " << root.string() << '\n'; return false; }
    fs::path dest = root / name;
    // Preserve user files. Repeated exports create numbered alternatives.
    for (int i = 1; fs::exists(dest, ec) && i < 1000; ++i) {
        dest = root / (fs::path(name).stem().string() + "-" + std::to_string(i) + fs::path(name).extension().string());
    }
    if (ec || fs::exists(dest)) { out << "Unable to find a free output filename.\n"; return false; }
    std::ofstream file(dest, std::ios::binary | std::ios::trunc);
    if (!file) { out << "Could not write " << dest.string() << '\n'; return false; }
    file << contents;
    file.close();
    if (!file) { out << "Writing setup script failed.\n"; return false; }
    out << "Generated: " << dest.string() << '\n';
    out << "This is a script to review and optionally execute manually; it has NOT been run.\n";
#ifdef _WIN32
    out << "WARNING: The supplied Windows script requests administrator elevation and changes system-wide packages, PATH and compiler defaults.\n";
    out << "It may move/remove a previously script-managed C:\\mingw64 folder; review it carefully before running.\n";
#else
    out << "The Linux script asks for consent before using sudo apt-get; inspect it before execution.\n";
#endif
    out << "GitHub: https://github.com/DGApproved/AI-Device-Bridge\n";
    return true;
}

void developer_menu() {
    for (;;) {
        std::cout << "\nDeveloper Menu\n"
                  << "1) View development environment setup instructions\n"
                  << "2) Download setup script to ~/Downloads\n"
                  << "3) GitHub repository link\n"
                  << "0) Return to main menu\nSelection: " << std::flush;
        std::string choice;
        if (!std::getline(std::cin, choice) || choice == "0") return;
        if (choice == "1") {
#ifdef _WIN32
            std::cout << "Windows: supplied setup-cpp-toolchain.bat uses winget to install Sublime Text, WinLibs MinGW-w64, CMake, Ninja.\n"
                      << "It requests administrator elevation and modifies machine-wide settings and tool locations.\n"
                      << "Review generated script before choosing to run it.\n";
#elif defined(__linux__)
            std::cout << "Linux: generated shell script supports apt-get (Debian/Ubuntu family).\n"
                      << "It prompts before invoking sudo to install build-essential, cmake and ninja-build.\n";
#else
            std::cout << "No setup script for this platform yet.\n";
#endif
            std::cout << "Repository: https://github.com/DGApproved/AI-Device-Bridge\n";
        } else if (choice == "2") {
            export_developer_script(developer_downloads_dir(), std::cout);
        } else if (choice == "3") {
            std::cout << "https://github.com/DGApproved/AI-Device-Bridge\n";
        } else std::cout << "Unknown selection.\n";
    }
}

void menu() {
    for (;;) {
        std::cout << "\n0) About / Versions\n1) Discover AI devices\n2) Diagnostics\n3) Find / Select Files\n4) Help\n5) Developer Menu\n6) Exit\nSelection: " << std::flush;
        std::string choice;
        if (!std::getline(std::cin, choice)) return;
        if (choice == "0") {
            std::cout << "Release: " << release_version << "\nSecurity record: " << security_record << " (historical baseline, not certification)\n";
        } else if (choice == "1") print_devices();
        else if (choice == "2") diagnostics_menu();
        else if (choice == "3") file_selection_menu();
        else if (choice == "4") help_menu();
        else if (choice == "5") developer_menu();
        else if (choice == "6") return;
        else std::cout << "Unknown selection.\n";
    }
}
}

int main(int argc, char** argv) {
    banner(enable_color());
    if (argc == 1) {
        if (output_is_terminal()) menu();
        return 0;
    }
    if (argc != 2) { help(); return 2; }
    const std::string arg(argv[1]);
    if (arg == "--help") help();
    else if (arg == "--links") website_links_report();
    else if (arg == "--repair-guide") repair_instructions();
    else if (arg == "--version") std::cout << release_version << '\n';
    else if (arg == "--security-version") std::cout << security_record << " (historical baseline; not a security certification)\n";
    else if (arg == "--devices") print_devices();
    else if (arg == "--diagnose") print_diagnostics();
    else if (arg == "--diagnose-system") diagnose_system();
    else if (arg == "--diagnose-device") diagnose_device();
    else if (arg == "--diagnose-driver") diagnose_driver();
    else if (arg == "--models") print_models();
    else if (arg == "--developer-menu") developer_menu();
    else if (arg == "--export-dev-setup") return export_developer_script(developer_downloads_dir(), std::cout) ? 0 : 1;
    else if (arg == "--menu") menu();
    else { std::cerr << "Unknown argument: " << arg << '\n'; help(); return 2; }
    return 0;
}
