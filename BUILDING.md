# Build AI Device Bridge

This initial proof of concept requires only a C++20 compiler and CMake 3.16 or newer.
It does **not** require a Hailo device, HailoRT, or a HEF file yet.

```sh
cmake -S . -B build
cmake --build build
./build/aidevicebridge
```

On Windows, the executable location depends on the CMake generator (for example, `build/Debug/aidevicebridge.exe`).

To run the automated greeting test:

```sh
ctest --test-dir build --output-on-failure
```

Expected output:

```text
Greetings, and Welcome to AI-Device-Bridge.
```

The eventual Hailo-8 backend will be added as an optional component so that the common core remains buildable independently of proprietary device tooling.
