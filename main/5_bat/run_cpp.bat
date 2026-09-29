@echo off
setlocal

rem Usage: run_cpp.bat [args...]   (builds first if the executable is missing)
cd /d "%~dp0..\.."

set "EXE=build\bin\TempSensorFwCpp.exe"

if not exist "%EXE%" (
    call "%~dp0build_cpp.bat" || exit /b 1
)

"%EXE%" %*
exit /b %ERRORLEVEL%
