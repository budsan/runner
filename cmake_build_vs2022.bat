@echo off
setlocal

cmake --preset windows-vs2022
if %ERRORLEVEL% NEQ 0 (
    echo CMake configure failed.
    exit /B 1
)

cmake --build --preset windows-vs2022
