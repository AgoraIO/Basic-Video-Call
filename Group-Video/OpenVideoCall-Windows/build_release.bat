@echo off
title qmake and nmake build prompt
set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC
set QTDIR=C:\Qt\Qt5.6.2\5.6\msvc2013
set PATH=%VCINSTALLDIR%\bin;%QTDIR%\bin;C:\Program Files\7-Zip;%PATH%
::shadow build
call "%VCINSTALLDIR%\vcvarsall.bat"
qmake OpenVideoCall.pro -spec win32-msvc2013 "CONFIG+=release" "CONFIG+=qml_release"
nmake
cd release
windeployqt OpenVideoCall.exe
OpenVideoCall.exe