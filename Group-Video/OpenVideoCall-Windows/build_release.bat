@echo off

title qmake and nmake build prompt
::C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC
::C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build;
set VCINSTALLDIR=%~1
::C:\Qt\5.12.2\msvc2017;C:\Qt\5.12.2\msvc2017_64
set QTDIR=%~2
::x86;x64
set Machine=%~3
echo vsdir: %VCINSTALLDIR%
echo qtdir:%QTDIR%
echo machine:%Machine%

set PATH=%VCINSTALLDIR%\bin;%QTDIR%\bin;C:\Program Files\7-Zip;%PATH%

::shadow build
set vsdevpath=%VCINSTALLDIR%\vcvarsall.bat
echo vsdevpath:%vsdevpath%
call "%VCINSTALLDIR%\vcvarsall.bat" %Machine%
qmake OpenVideoCall.pro "CONFIG+=release" "CONFIG+=qml_release"
nmake
cd release
windeployqt OpenVideoCall.exe