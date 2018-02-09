@echo off
	@set Path_=%1%
  echo %Path_%
	for,/f,"skip=4 tokens=1,2,*",%%a,in,('reg query HKCU\Environment /v Path'),do,( 
  echo %%c
	rem @set PathAll_=%%c
	)
  
	REM echo %PathAll_%|find /i %Path_% && set IsNull=true|| set IsNull=false
	REM if not %IsNull%==true (
    REM echo %PathAll_%;%Path_%
		  REM reg add HKCU\Environment /v Path /t REG_EXPAND_SZ /d %PathAll_%;%Path_% /f
	REM )
pause