@echo off
setlocal

rem Usage: run_c.bat [args...]   (builds first if the executable is missing)
cd /d "%~dp0..\.."

set "EXE=build\bin\TempSensorFw.exe"

if not exist "%EXE%" (
    call "%~dp0build_c.bat" || exit /b 1
)

"%EXE%" %*
exit /b %ERRORLEVEL%
