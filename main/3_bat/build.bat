@echo off
setlocal enabledelayedexpansion

rem Usage: build.bat [Debug|Release] [extra cmake args...]   (default: Debug)
rem The scripts live in main/3_bat; every path below is relative to the root.
cd /d "%~dp0..\.."

set BUILD_TYPE=%~1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Debug

rem Everything after the build type is forwarded to CMake verbatim.
rem (Args are sliced out of %* as a string: SHIFT does not update %*, and "="
rem  is a token delimiter, so -DFOO=BAR would be split across %1 and %2.)
set "EXTRA_ARGS="
set "ALL_ARGS=%*"
if not "%~1"=="" if defined ALL_ARGS set "EXTRA_ARGS=!ALL_ARGS:*%~1=!"

rem Locate a toolchain: prefer one already on PATH, otherwise try known roots.
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

rem This is a host build: pin the native compiler explicitly, so a cross
rem toolchain earlier on PATH (or a cache written by another tool) cannot be
rem picked up instead.
rem SIM_ENABLE is passed on every configure so it cannot stay stuck in the cache:
rem a plain build.bat always returns to the host simulation build, while an
rem explicit -DSIM_ENABLE=OFF in the extra args still wins, being later.
set "CMAKE_ARGS=-G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DSIM_ENABLE=ON!EXTRA_ARGS!"

call :configure_and_build
if errorlevel 1 (
    echo.
    echo Build directory is stale or was written by another toolchain; starting clean.
    if exist build rmdir /s /q build
    call :configure_and_build
    if errorlevel 1 exit /b 1
)

echo.
echo Build succeeded: build\bin\TempSensorFw.exe
exit /b 0

:configure_and_build
cmake -S . -B build %CMAKE_ARGS%
if errorlevel 1 exit /b 1
cmake --build build
if errorlevel 1 exit /b 1
goto :eof
