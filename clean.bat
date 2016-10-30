@echo off
if "%1" == "" goto :usage
wmake clean BUILD=%1
goto :eof

:usage
echo clean [debug / release]
echo.

:eof
