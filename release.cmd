@echo off
pushd "%~dp0"

powershell Compress-7Zip "Release"  -ArchiveFileName "jack-win32.zip"
powershell Compress-7Zip "x64\Release"  -ArchiveFileName "jack-win64.zip"

:exit
popd
@echo on

