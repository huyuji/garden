const HKEY_CURRENT_USER = &H80000001
strComputer = "."  
Set oReg=GetObject("winmgmts:{impersonationLevel=impersonate}!\\" & strComputer & "\root\default:StdRegProv")  
strKeyPath = "Software\Microsoft\Windows\CurrentVersion\Explorer"  
strValueName = "link"
strValue = Array(0,0,0,0)
oReg.SetBinaryValue HKEY_CURRENT_USER,strKeyPath,strValueName,strValue  
WScript.Echo "ok"