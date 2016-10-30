@echo off
if "%1" == "" goto :usage
wmake -e BUILD=%1
goto :eof

:usage
echo build [debug / release]
echo.

:eof
