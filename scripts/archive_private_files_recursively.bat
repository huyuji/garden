@echo off
set dir=%1%
set password=%2%

pushd %dir%
for /R %%f in (*.*) do (
  if "%%~xf" NEQ ".zip" (
    echo "%%f"
    7z a -p%password% "%%~df\%%~pf%%~nf.zip" "%%f" || GOTO :end
    rem del "%%f"
  )
)

:end
popd
