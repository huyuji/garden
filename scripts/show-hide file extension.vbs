' Script to toggle Windows Explorer display of hidden files,
' super-hidden files, and file name extensions

Option Explicit
Dim dblHiddenData, strHiddenKey, strSuperHiddenKey, strFileExtKey
Dim strKey, WshShell
On Error Resume Next

strKey = "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced"
strFileExtKey = strKey & "\HideFileExt"

Set WshShell = WScript.CreateObject("WScript.Shell")
dblHiddenData = WshShell.RegRead(strFileExtKey)

If dblHiddenData = 1 Then
	WshShell.RegWrite strFileExtKey, 0, "REG_DWORD"
	WScript.Echo "��ʾ��׺����ˢ��"

Else
	WshShell.RegWrite strFileExtKey, 1, "REG_DWORD"
	WScript.Echo "���غ�׺����ˢ��"

End If