@echo off

title qmake and nmake build prompt
set VCINSTALLDIR=%~1
set QTDIR=%~2
echo vsdir: %VCINSTALLDIR%
echo qtdir:%QTDIR%
set PATH=%VCINSTALLDIR%\bin;%QTDIR%\bin;C:\Program Files\7-Zip;%PATH%
::shadow build
set vsdevpath=%VCINSTALLDIR%\vcvarsall.bat
echo vsdevpath:%vsdevpath%
call "%VCINSTALLDIR%\vcvarsall.bat"
qmake OpenVideoCall.pro -spec win32-msvc2013 "CONFIG+=release" "CONFIG+=qml_release"
nmake
cd release
windeployqt OpenVideoCall.exe
OpenVideoCall.exe