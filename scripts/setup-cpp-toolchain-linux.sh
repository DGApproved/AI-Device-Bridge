#!/usr/bin/env bash
# #Generation-Assisted-With-GPT-6#
# #Generation-Assisted-with-OpenAI#
# Attribution records AI assistance; later human edits do not invalidate it.
# AI Device Bridge — OPTIONAL Linux development environment setup.
# Generated for user review. No changes are made until you choose to run it.
# Supported automated install path: Debian / Ubuntu / derivatives using apt.
set -euo pipefail
printf '%s\n' 'AI Device Bridge Linux developer prerequisites'
if ! command -v apt-get >/dev/null 2>&1; then
  printf '%s\n' 'Unsupported automatic setup: apt-get not available.'
  printf '%s\n' 'Install a C++20 compiler, CMake, make or Ninja, and an editor through your distribution package manager.'
  exit 1
fi
printf '%s\n' 'This script will ask for sudo to install build-essential, cmake, and ninja-build.'
read -r -p 'Proceed with package installation? [y/N] ' response
case "$response" in y|Y|yes|YES) ;; *) echo 'Cancelled'; exit 0;; esac
sudo apt-get update
sudo apt-get install -y build-essential cmake ninja-build
printf '%s\n' 'Prerequisites installed. Clone the AI Device Bridge repository:'
printf '%s\n' 'git clone https://github.com/DGApproved/AI-Device-Bridge.git'
printf '%s\n' 'cd AI-Device-Bridge'
printf '%s\n' 'cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release'
printf '%s\n' 'cmake --build build'
printf '%s\n' 'ctest --test-dir build --output-on-failure'
