<!-- #Generation-Assisted-With-GPT-6# -->
<!-- #Generation-Assisted-with-OpenAI# -->
# AI Device Bridge — release 0002 (pre-alpha)

Requires a C++20 compiler and CMake 3.16+. No HailoRT dependency yet.

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/aidevicebridge --menu
```

Windows CMake multi-configuration generators generally produce `build/Debug/aidevicebridge.exe` and may require `cmake --build build --config Release` and `ctest --test-dir build -C Release`.

Commands: `--help`, `--version`, `--security-version`, `--devices`, `--diagnose`, `--models`, `--menu`. With no arguments, a menu opens in a terminal; when output is redirected, only the greeting and banner are printed.

Set `AIDB_MODEL_DIR` to a directory containing `.hef` files to list their names. This is discovery **only**: no model validation or execution occurs. PCI scanning is supported on Linux and Windows and only recognizes Hailo PCI vendor ID `0x1e60`. It does not verify a specific accelerator model, driver usability or inference capability. Windows uses SetupAPI to enumerate present PnP devices with Hailo PCI vendor ID 1e60; it can identify device ID 2864 but does not prove inference functionality.

`--security-version` identifies the foundational `CORRECTIONS.md` record (`0000-0000-0000-0001`). It is **not** a guarantee that vulnerabilities have been fixed or a security certification.

Color is used for the pre-alpha banner only on supported interactive terminals; set `NO_COLOR=1` to disable it.

No drivers, firmware, configuration files or device states are changed by this release.

## Option 3 — Find / Select Files

The interactive menu's `3) Find / Select Files` opens a submenu for `*.hef`, `*.onnx`, `*.gguf`, `*.safetensors`, `*.tflite`, or all listed formats. Option 7 allows choosing an existing directory (for the current session), while `AIDB_MODEL_DIR` sets the initial directory. After listing matches, entering a file number reports its absolute path; **no file is opened, parsed, loaded, or executed**. The search examines only files directly in the selected directory, not subdirectories. The original `--models` command continues listing HEFs only.

## Option 2 — Diagnostics submenu (0002 revision)

The interactive `2) Diagnostics` menu now opens:

- `1) System`: host OS, PCI sysfs (Linux), presence of CMake / C++ compiler (optional build-only tools), `hailortcli` presence, and explicit shared-library untested status.
- `2) Device`: Hailo PCIe vendor enumeration, `/dev/hailo0` existence, read/write permissions (Linux); firmware health and functional inference remain untested.
- `3) Driver`: Linux sysfs PCI driver binding, `hailo_pci` module presence, and CLI presence; runtime compatibility remains untested.
- `4) Run all diagnostics`: runs all three groups.
- `0) Return to main menu`.

Non-interactive commands: `--diagnose` (all), `--diagnose-system`, `--diagnose-device`, and `--diagnose-driver`. Each check reports `PASS`, `MISSING`, `WARNING`, or `UNTESTED` with an explanation. A `PASS` for enumeration, executable presence, or driver binding does **not** guarantee functioning accelerator inference. Windows tests SetupAPI device presence, PnP started status, and reported driver service; functional inference and runtime compatibility are still untested. These tests are observational and never install, load, unload, or modify drivers or firmware.

## Option 4 — Help submenu (0002 revision)

`4) Help` opens `1) Website Links`, `2) Repair / Installation Commands`, `3) General Help / Command-Line Options`, and `0) Return`.

Website Links includes the project's repository and official Hailo resources. `--links` prints them non-interactively.

Repair / Installation Commands prints ordered, **non-executing** instructions adjusted to observations such as compiler/CMake availability, PCIe vendor detection, `/dev/hailo0`, kernel module presence, and `hailortcli` availability. `--repair-guide` prints them non-interactively. It recommends package-manager commands only for recognized Debian/Ubuntu-family Linux environments and uses placeholders for official device packages. It never downloads, installs, repairs, executes commands, changes configuration, or rebinds drivers. Check official versions and package requirements before manually running any suggested commands. Windows displays PowerShell checks, optional winget installation suggestions, and an MSYS2 UCRT64 MinGW/CMake build procedure. Nothing is automatically installed.

The existing command-line help is retained under option 3 and `--help`.

## Device-aware Website Links (0002 revision)

Under `4) Help > 1) Website Links`, the menu is populated from the same read-only PCIe discovery used by `1) Discover AI devices`. A detected PCI vendor/device ID `1e60:2864` produces `1) Hailo 8.0 M.2`; selecting it displays official Hailo product, developer, HailoRT Hailo-8 branch, Model Zoo v2.x, and community links. PCIe identification does not establish physical M.2 form factor, driver health or inference capability. Other detected Hailo devices are shown without a device-specific links menu until an appropriate profile is added. When no supported device is found, no device-specific menu is advertised.

`--links` remains noninteractive and prints the detected-device link summary. This program never automatically opens a website or installs any software.

## Windows development (MinGW, CMake, winget, Sublime Text)

Open **PowerShell** and optionally install the development tools (verify package identity before executing):

```powershell
winget install --id MSYS2.MSYS2 --exact
winget install --id SublimeHQ.SublimeText.4 --exact
# Optional if CMake will not be installed through MSYS2:
winget install --id Kitware.CMake --exact
```

Start **MSYS2 UCRT64** (not the plain MSYS shell). Update packages, reopen the UCRT64 shell if prompted, and install the native Windows MinGW toolchain:

```bash
pacman -Syu
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```

Navigate to the cloned repository within the UCRT64 terminal and build:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
./build/aidevicebridge.exe --diagnose
./build/aidevicebridge.exe --menu
```

Use Sublime Text to edit `src/main.cpp` and other source files; it is not required for compilation.

Windows discovery calls SetupAPI and Configuration Manager APIs through the preexisting `setupapi` and `cfgmgr32` CMake link libraries. It does not use PowerShell subprocesses to discover devices, and does not require elevation to enumerate devices. Device Manager may report a present device even if the corresponding inference runtime is not installed or functioning. Model file selection uses portable `std::filesystem` and accepts Windows paths. `AIDB_MODEL_DIR` is supported on both systems.

**Windows testing status:** Windows compilation and device testing must be performed on a Windows system; a Linux build/test only validates the Linux branch. The official HailoRT project describes Windows support, but the correct driver/runtime release for each accelerator must be confirmed with Hailo's documentation.

## Option 5 — Developer Menu

The main-menu `5) Developer Menu` displays platform-specific setup guidance and a link to the project at https://github.com/DGApproved/AI-Device-Bridge. Option `2) Download setup script to ~/Downloads` creates the script in the current user's Downloads directory without executing it or overwriting a preexisting file. Repeated exports use numbered filenames. CLI equivalent: `--export-dev-setup`.

The Windows script is the user-provided `setup-cpp-toolchain.bat`, embedded verbatim. It **requests administrative elevation, installs tools using winget, adjusts system-wide PATH and environment variables, relocates WinLibs under C:\mingw64, and runs a hello-world build test**. Review before running. It is not automatically executed by AI Device Bridge.

The Linux script is a new minimal Debian/Ubuntu-family `apt-get` helper that prompts before invoking `sudo`; non-apt systems receive an explanatory message when executed. It too is **generated only**, never run by the bridge. To edit the scripts for future builds, update `scripts/` and the matching embedded strings in `src/developer_scripts.hpp`.
