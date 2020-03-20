' Script to toggle Windows Explorer display of hidden files,
' super-hidden files, and file name extensions

Option Explicit
Dim dblHiddenData, strHiddenKey, strSuperHiddenKey, strFileExtKey
Dim strKey, WshShell
On Error Resume Next

strKey = "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced"
strHiddenKey = strKey & "\Hidden"
strSuperHiddenKey = strKey & "\ShowSuperHidden"

Set WshShell = WScript.CreateObject("WScript.Shell")
dblHiddenData = WshShell.RegRead(strHiddenKey)

If dblHiddenData = 2 Then
	WshShell.RegWrite strHiddenKey, 1, "REG_DWORD"
	WshShell.RegWrite strSuperHiddenKey, 1, "REG_DWORD"
	WScript.Echo "显示文件，请刷新"

Else
	WshShell.RegWrite strHiddenKey, 2, "REG_DWORD"
	WshShell.RegWrite strSuperHiddenKey, 0, "REG_DWORD"
	WScript.Echo "隐藏文件，请刷新"

End If