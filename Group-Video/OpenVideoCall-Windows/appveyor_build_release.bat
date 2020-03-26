@echo off

title qmake and nmake build prompt
set SDKVersion=%~1
set Machine=%~2
echo %Machine%
if %Machine% == x86 (
  set QTDIR=C:\Qt\5.13.2\msvc2017
) else (
  set QTDIR=C:\Qt\5.13.2\msvc2017_64
)

set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build

call "%VCINSTALLDIR%\vcvarsall.bat" x86
%QTDIR%\bin\qmake.exe OpenVideoCall.pro "CONFIG+=release" "CONFIG+=qml_release"
nmake
cd release
del *.h
del *.cpp
del *.obj
%QTDIR%\bin\windeployqt OpenVideoCall.exe
7z a -tzip -r OpenVideoCall_Win_v%SDKVersion%(%Machine%)