@echo off
setlocal enabledelayedexpansion

rem Usage: build_cpp.bat [Debug|Release] [extra cmake args...]   (default: Debug)
cd /d "%~dp0..\.."

set BUILD_TYPE=%~1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Debug

rem Arguments after the build type go to CMake as they are.
set "EXTRA_ARGS="
set "ALL_ARGS=%*"
if not "%~1"=="" if defined ALL_ARGS set "EXTRA_ARGS=!ALL_ARGS:*%~1=!"

where gcc >nul 2>nul
if errorlevel 1 (
    for %%D in (
        "C:\MinGW\bin"
        "C:\msys64\mingw64\bin"
        "C:\msys64\ucrt64\bin"
        "C:\ProgramData\mingw64\mingw64\bin"
    ) do (
        if exist "%%~D\gcc.exe" (
            set "PATH=%%~D;!PATH!"
            goto :found_gcc
        )
    )
    echo ERROR: gcc was not found on PATH and no known MinGW install was located.
    echo        Add your toolchain's bin directory to PATH and re-run.
    exit /b 1
)
:found_gcc

set "CMAKE_ARGS=-G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DSIM_ENABLE=ON!EXTRA_ARGS!"

rem A failed configure usually means a stale cache: retry once from clean.
call :configure
if errorlevel 1 (
    echo.
    echo Build directory is stale or was written by another toolchain; starting clean.
    if exist build rmdir /s /q build
    call :configure
    if errorlevel 1 exit /b 1
)

cmake --build build --target TempSensorFwCpp
if errorlevel 1 exit /b 1

echo.
echo Build succeeded: build\bin\TempSensorFwCpp.exe
exit /b 0

:configure
cmake -S . -B build %CMAKE_ARGS%
if errorlevel 1 exit /b 1
goto :eof
