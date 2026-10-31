#pragma once
// #Generation-Assisted-With-GPT-6#
// #Generation-Assisted-with-OpenAI#
// Embedded setup scripts; generated from scripts/ with no third-party runtime dependencies.
namespace aidb_scripts {
inline constexpr const char* windows = R"AIDBWIN0002(@echo off
rem #Generation-assisted-With-Claude#
rem #Generation-Assisted-with-OpenAI#
rem Attribution records AI assistance; later human edits do not invalidate it.
setlocal
title C++ Toolchain Setup (Sublime Text + MinGW + CMake)

rem ============================================================
rem  setup-cpp-toolchain.bat
rem  System-wide install of Sublime Text, MinGW-w64 (WinLibs),
rem  CMake and Ninja via winget. Asks for administrator rights
rem  ONCE at the start, then everything runs elevated.
rem  Sets CMake to use MinGW + Ninja by default for all users,
rem  then compiles and runs a Hello World test.
rem  Exit code: 0 = BUILD SUCCEEDED, 1 = failure.
rem ============================================================

rem ---- Elevate once (single UAC prompt) ----------------------
fltmc >nul 2>&1
if not errorlevel 1 goto :is_admin
echo Requesting administrator rights...
powershell -NoProfile -ExecutionPolicy Bypass -Command "$p = Start-Process -FilePath '%~f0' -Verb RunAs -Wait -PassThru; exit $p.ExitCode"
exit /b %errorlevel%
:is_admin

set "WG=winget install -e --silent --accept-package-agreements --accept-source-agreements"
set "SYSENV=HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment"

echo.
echo === Checking for winget ===
where winget >nul 2>&1
if errorlevel 1 (
    echo winget not found. Install "App Installer" from the Microsoft Store, then rerun.
    goto :fail
)
echo winget found. Running as administrator.

rem ------------------------------------------------------------
echo.
echo === [1/4] Sublime Text ===
set "SUBL_DIR="
if exist "%ProgramFiles%\Sublime Text\sublime_text.exe" set "SUBL_DIR=%ProgramFiles%\Sublime Text"
if defined SUBL_DIR goto :subl_ok
echo Not found. Installing latest Sublime Text (all users)...
call :WingetInstall SublimeHQ.SublimeText.4
if exist "%ProgramFiles%\Sublime Text\sublime_text.exe" set "SUBL_DIR=%ProgramFiles%\Sublime Text"
if not defined SUBL_DIR (
    echo Sublime Text install failed.
    goto :fail
)
:subl_ok
echo Sublime Text: %SUBL_DIR%
call :AddSystemPath "%SUBL_DIR%"

rem ------------------------------------------------------------
echo.
echo === [2/4] MinGW-w64 (WinLibs GCC) ===
rem GCC's linker breaks when its real location has spaces
rem (C:\Program Files\...), and GCC follows links back to its real
rem location. So the real files live in C:\mingw64, and the winget
rem package folder becomes a junction pointing there. Winget still
rem sees WinLibs as installed. After a "winget upgrade" (which puts a
rem fresh real folder back in Program Files), rerun this script.
set "WL_ROOT="
call :FindWinLibs
if defined WL_ROOT goto :wl_installed
echo Not found. Installing latest WinLibs MinGW-w64 (UCRT, POSIX threads, all users)...
call :WingetInstall BrechtSanders.WinLibs.POSIX.UCRT
call :FindWinLibs
if not defined WL_ROOT (
    echo MinGW install failed or g++.exe could not be located.
    goto :fail
)
:wl_installed
echo WinLibs package folder: %WL_ROOT%
set "MARK=C:\mingw64\.winget-relocated"

rem Already relocated? (package folder is a junction and C:\mingw64 is real)
fsutil reparsepoint query "%WL_ROOT%" >nul 2>&1
if errorlevel 1 goto :need_move
if exist "C:\mingw64\bin\g++.exe" (
    echo Already relocated: files in C:\mingw64, package folder links there.
    goto :relocated
)
echo Package folder is a link but C:\mingw64 is missing; reinstall needed.
rmdir "%WL_ROOT%"
call :WingetInstall BrechtSanders.WinLibs.POSIX.UCRT
set "WL_ROOT="
call :FindWinLibs
if not defined WL_ROOT (
    echo MinGW reinstall failed.
    goto :fail
)

:need_move
rem Package folder holds real files (fresh install or upgrade). Clear C:\mingw64.
if not exist "C:\mingw64" goto :do_move
fsutil reparsepoint query "C:\mingw64" >nul 2>&1
if errorlevel 1 goto :old_real
echo   Removing old junction C:\mingw64
rmdir "C:\mingw64"
goto :do_move
:old_real
if exist "C:\mingw64\.winget-relocated" goto :old_ours
if exist "C:\mingw64\.winget-mirror" goto :old_ours
echo C:\mingw64 already exists and was not created by this script.
echo Rename or remove it, then rerun this script.
goto :fail
:old_ours
echo   Removing previous version in C:\mingw64
rmdir /s /q "C:\mingw64"
if exist "C:\mingw64" (
    echo Could not remove old C:\mingw64. Close programs using it and rerun.
    goto :fail
)

:do_move
echo Moving WinLibs to C:\mingw64 ...
move "%WL_ROOT%" "C:\mingw64" >nul
if not exist "C:\mingw64\bin\g++.exe" (
    echo Move failed. Close any programs using MinGW and rerun.
    goto :fail
)
mklink /J "%WL_ROOT%" "C:\mingw64" >nul
if errorlevel 1 (
    echo Could not create the junction in the winget package folder.
    goto :fail
)
>"%MARK%" echo Real WinLibs files; winget package folder is a junction to here. Managed by setup-cpp-toolchain.bat
echo Moved to C:\mingw64 and linked package folder there.

:relocated
set "MINGW_BIN=C:\mingw64\bin"
call :CleanWinLibsPath
:mingw_ok
echo MinGW: %MINGW_BIN%
set "PATH=%MINGW_BIN%;%PATH%"
call :AddSystemPath "%MINGW_BIN%"

rem ------------------------------------------------------------
echo.
echo === [3/4] CMake ===
set "CMAKE_BIN="
if exist "%ProgramFiles%\CMake\bin\cmake.exe" set "CMAKE_BIN=%ProgramFiles%\CMake\bin"
if defined CMAKE_BIN goto :cmake_ok
echo Not found. Installing latest CMake (all users)...
call :WingetInstall Kitware.CMake
if exist "%ProgramFiles%\CMake\bin\cmake.exe" set "CMAKE_BIN=%ProgramFiles%\CMake\bin"
if not defined CMAKE_BIN (
    echo CMake install failed.
    goto :fail
)
:cmake_ok
echo CMake: %CMAKE_BIN%
set "PATH=%PATH%;%CMAKE_BIN%"
call :AddSystemPath "%CMAKE_BIN%"

rem ------------------------------------------------------------
echo.
echo === [4/4] Ninja (build tool CMake will use) ===
where ninja >nul 2>&1
if not errorlevel 1 goto :ninja_ok
echo Not found. Installing latest Ninja (all users)...
call :WingetInstall Ninja-build.Ninja
set "PATH=%PATH%;%ProgramFiles%\WinGet\Links;%LOCALAPPDATA%\Microsoft\WinGet\Links"
where ninja >nul 2>&1
if errorlevel 1 (
    echo Ninja install failed.
    goto :fail
)
call :AddSystemPath "%ProgramFiles%\WinGet\Links"
:ninja_ok
echo Ninja found.

rem ------------------------------------------------------------
echo.
echo === Setting system-wide CMake defaults (MinGW + Ninja) ===
rem setx /M also tells Windows the environment changed, so new windows see the PATH update
setx CMAKE_GENERATOR Ninja /M >nul
setx CC gcc /M >nul
setx CXX g++ /M >nul
set "CMAKE_GENERATOR=Ninja"
set "CC=gcc"
set "CXX=g++"
echo CMAKE_GENERATOR=Ninja, CC=gcc, CXX=g++ (all users)

rem ------------------------------------------------------------
echo.
echo === Hello World build test ===
set "HELLO=%TEMP%\cpp_hello_test"
if exist "%HELLO%" rmdir /s /q "%HELLO%"
mkdir "%HELLO%"

>"%HELLO%\CMakeLists.txt" echo cmake_minimum_required(VERSION 3.16)
>>"%HELLO%\CMakeLists.txt" echo project(hello LANGUAGES CXX)
>>"%HELLO%\CMakeLists.txt" echo add_executable(hello main.cpp)
>>"%HELLO%\CMakeLists.txt" echo target_link_options(hello PRIVATE -static)

>"%HELLO%\main.cpp" echo #include ^<cstdio^>
>>"%HELLO%\main.cpp" echo int main() {
>>"%HELLO%\main.cpp" echo     std::printf("Hello, world\n");
>>"%HELLO%\main.cpp" echo     return 0;
>>"%HELLO%\main.cpp" echo }

cmake -S "%HELLO%" -B "%HELLO%\build" -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
    echo CMake configure step failed.
    goto :fail
)
cmake --build "%HELLO%\build"
if errorlevel 1 (
    echo Compile step failed.
    goto :fail
)

"%HELLO%\build\hello.exe" > "%HELLO%\out.txt"
if errorlevel 1 (
    echo Test program returned an error.
    goto :fail
)
set "OUT="
set /p OUT=<"%HELLO%\out.txt"
echo Program output: %OUT%
if not "%OUT%"=="Hello, world" (
    echo Unexpected output.
    goto :fail
)

echo.
echo ============================================
echo   BUILD SUCCEEDED
echo   Close and reopen Command Prompt / Sublime
echo   Text so they pick up the new settings.
echo ============================================
echo.
pause
endlocal
exit /b 0

:fail
echo.
echo ============================================
echo   BUILD FAILED - see messages above
echo ============================================
echo.
pause
endlocal
exit /b 1

rem ============================================================
rem  Subroutines
rem ============================================================

:WingetInstall
rem Try an all-users install first; if the package has no machine-scope
rem installer, retry without the scope flag (still elevated, so installers
rem like Sublime Text's still go to Program Files).
%WG% --scope machine --id %1
if not errorlevel 1 exit /b 0
echo   Machine-scope install not available for %1, retrying default scope...
%WG% --id %1
exit /b %errorlevel%

:FindWinLibs
rem Sets WL_ROOT to the winget WinLibs "mingw64" folder, if installed.
for /d %%D in ("%ProgramFiles%\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_*") do (
    if exist "%%D\mingw64\bin\g++.exe" set "WL_ROOT=%%D\mingw64"
)
if defined WL_ROOT exit /b 0
for /d %%D in ("%LOCALAPPDATA%\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_*") do (
    if exist "%%D\mingw64\bin\g++.exe" set "WL_ROOT=%%D\mingw64"
)
exit /b 0

:CleanWinLibsPath
rem Remove the real (space-containing) WinLibs folders from the system and
rem user PATH so GCC is only ever started through C:\mingw64.
powershell -NoProfile -ExecutionPolicy Bypass -Command "foreach ($k in 'HKLM:\SYSTEM\CurrentControlSet\Control\Session Manager\Environment','HKCU:\Environment') { $i=Get-Item $k; $p=$i.GetValue('Path','','DoNotExpandEnvironmentNames'); if ($p) { $n=($p -split ';' | Where-Object { $_ -and $_ -notlike '*BrechtSanders.WinLibs*' }) -join ';'; if ($n -ne $p) { Set-ItemProperty $k -Name Path -Value $n -Type ExpandString } } }"
exit /b 0

:AddSystemPath
rem Adds %~1 to the front of the SYSTEM Path in the registry if missing.
rem Uses reg.exe instead of "setx PATH /M" to avoid the 1024-character truncation.
set "SPATH="
for /f "tokens=2,*" %%A in ('reg query "%SYSENV%" /v Path 2^>nul') do set "SPATH=%%B"
echo ;%SPATH%; | find /i ";%~1;" >nul
if not errorlevel 1 (
    echo   Already on system PATH: %~1
    exit /b 0
)
reg add "%SYSENV%" /v Path /t REG_EXPAND_SZ /d "%~1;%SPATH%" /f >nul
echo   Added to system PATH: %~1
exit /b 0
)AIDBWIN0002";
inline constexpr const char* linux_script = R"AIDBLNX0002(#!/usr/bin/env bash
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
)AIDBLNX0002";
}
