@echo off
setlocal

if "%~1"=="" (
    echo Usage: %~nx0 ^<preset-name^>
    exit /B 1
)

set "VCVARSALL=C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat"
if not exist "%VCVARSALL%" (
    echo Could not find vcvarsall.bat at "%VCVARSALL%".
    exit /B 1
)

call "%VCVARSALL%" x64
if %ERRORLEVEL% NEQ 0 exit /B 1

cmake --preset %1
if %ERRORLEVEL% NEQ 0 (
    echo CMake configure failed.
    exit /B 1
)

cmake --build --preset %1
