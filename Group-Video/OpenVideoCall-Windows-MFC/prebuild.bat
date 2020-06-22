set sdkversion=small

set Machine=%~1
set absolute_path=%~2
cd %absolute_path%

if %sdkversion% == origin ( exit )


if %sdkversion% == small (
  set sdkinclude=libs\include
  set sdkdll=libs\x86
  set sdklib=libs\x86
  if %Machine% == x64 (
    set sdkdll=libs\x86_64
    set sdklib=libs\x86_64
  )
)

if not exist sdk (
  mkdir sdk
)

if exist sdk ( 
  pushd sdk
  if not exist dll (
    mkdir dll
  )
 
  if not exist lib (
    mkdir lib
  )
  if not exist include (
    mkdir include
  )
  popd
)

if %sdkversion% == small (
  if exist %sdkinclude% (
    copy %sdkinclude%\*.h sdk\include
  )

  if exist %sdklib% (
    copy %sdklib%\*.lib sdk\lib /y
  )
  if exist %sdkdll% (
    copy %sdkdll%\*.dll sdk\dll /y
  )
)
pause