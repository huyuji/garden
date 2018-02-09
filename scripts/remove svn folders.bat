@echo off
for /r %%a in (.) do if exist %%a\.svn echo %%a\.svn rd /s /q %%a\.svn
echo ok
pause >nul