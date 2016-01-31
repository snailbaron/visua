@echo off
setlocal EnableExtensions
setlocal EnableDelayedExpansion

set "build_dir=%~dp0build"

if not exist "%build_dir%" mkdir "%build_dir%"
cd "%build_dir%"

cmake -G "Visual Studio 14 2015 Win64" ..\src

pause

endlocal
endlocal
