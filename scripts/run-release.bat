@echo off
set EXE_NAME=chip8vm.exe
pushd %~dp0\..\build\chip8vm\Release
if exist %EXE_NAME% (
    %EXE_NAME%
) else (
    echo Release executable "%EXE_NAME%" not found!
)
popd

