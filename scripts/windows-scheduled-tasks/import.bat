@echo off
echo Importing All Tasks
echo.
schtasks.exe /create /TN "user logoff" /XML logoff.xml /ru "QA Test" /rp masterPI
schtasks.exe /create /TN "user logon" /XML logon.xml /ru "QA Test" /rp masterPI
schtasks.exe /create /TN "power off" /XML "power off.xml" /ru "QA Test" /rp masterPI
schtasks.exe /create /TN "power on" /XML "power on.xml" /ru "QA Test" /rp masterPI
schtasks.exe /create /TN "remote connect" /XML "remote connect.xml" /ru "QA Test" /rp masterPI
schtasks.exe /create /TN "remote disconnect" /XML "remote disconnect.xml" /ru "QA Test" /rp masterPI
schtasks.exe /create /TN "session lock" /XML "session lock.xml" /ru "QA Test" /rp masterPI
schtasks.exe /create /TN "session unlock" /XML "session unlock.xml" /ru "QA Test" /rp masterPI
echo.
echo Importing Done
echo.
pause 