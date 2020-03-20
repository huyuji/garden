@echo off

call :treeProcess
goto :eof

:treeProcess
for %%f in (*.*) do (
  7z x "%%f" -r
  del "%%f"
)
for /D %%d in (*) do (
    pushd %%d
    call :treeProcess
    popd ..
)
exit /b