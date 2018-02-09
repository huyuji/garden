echo off
set dir=%1%
set password=%2%
for /f "delims=" %%i in ('dir /b "%dir%"') do (7z a "%dir%\%%i".zip "%dir%\%%i" -p%password%)
pause