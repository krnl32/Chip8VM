@echo off
set EXE_NAME=chip8vm.exe
pushd %~dp0\..\build\chip8vm\Debug
if exist %EXE_NAME% (
    %EXE_NAME%
) else (
    echo Debug executable "%EXE_NAME%" not found!
)
popd

