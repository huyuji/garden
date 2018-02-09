@echo off
set dir=%1%
set password=%2%

for /R %%f in (%dir%) do (
  7z a -p%password% "%%f.zip" "%%f"
  del "%%f"
)