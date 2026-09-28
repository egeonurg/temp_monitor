@echo off
setlocal

rem The scripts live in main/3_bat; every path below is relative to the root.
cd /d "%~dp0..\.."

if exist build (
    rmdir /s /q build
    echo Removed build directory.
) else (
    echo Nothing to clean.
)
