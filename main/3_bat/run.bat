@echo off
setlocal

rem Usage: run.bat [args...]   (builds first if the executable is missing)
rem The scripts live in main/3_bat; every path below is relative to the root.
cd /d "%~dp0..\.."

set "EXE=build\bin\TempSensorFw.exe"

if not exist "%EXE%" (
    call "%~dp0build.bat" || exit /b 1
)

"%EXE%" %*
exit /b %ERRORLEVEL%
