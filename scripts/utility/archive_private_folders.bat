@echo off
set dir=%1%
set password=%2%

pushd %dir%
for /D %%d in (*) do (
    echo "%%d"
    7z a -p%password% "%%d.zip" "%%d" || GOTO :end
    rem rd /s /q "%%d"
)

:end
popd
