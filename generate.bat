@echo off
setlocal EnableExtensions
setlocal EnableDelayedExpansion

set "build_dir=%~dp0build"

if exist "%build_dir%" rmdir /s /q "%build_dir%"
mkdir "%build_dir%"
cd "%build_dir%"

cmake -G "Visual Studio 14 2015 Win64" ..\src

endlocal
endlocal
