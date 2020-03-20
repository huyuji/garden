Option Explicit
Dim oldpath,newpath

''''''''''''''''说明'''''''''''''''''
'作用：批量修改快捷方式的“目标”和“起始位置”
'使用方法：和需要修改的快捷方式放在同一个文件夹执行
'''''''''''''''说明完'''''''''''''''''

oldpath = "sforge.sspg.lab.emc.com"           '设置原路径中将被替换的内容
newpath = "teamforge6.usd.lab.emc.com"   '设置新路径中要使用的内容

Dim Wsh,fso
Set Wsh = WScript.CreateObject("WScript.Shell")
Set fso = CreateObject("Scripting.FileSystemObject")
Dim ji_1,ji_2,Folder
ji_1 = 0
ji_2 = 0
Folder = Wsh.CurrentDirectory
if Ask("将要修改"&chr(34)& Folder &chr(34)&"里的所有快捷方，是否继续") then
    Dim f,fc,f1,ext
    Set f = fso.GetFolder(Folder)
    Set fc = f.Files
    For Each f1 in fc
       ext = LCase(fso.GetExtensionName(f1))
       if ext = "url" then
          ji_1 = ji_1 + 1
          call Doit(f1)
       end if
    Next
end if

Set WSH = Nothing
msgbox "找到 "&ji_1&" 个快捷方式"&vbCrLf&"修改 "&ji_2&" 个快捷方式",64,"执行完毕"
WScript.quit

Sub Doit(strlnk)
Dim oShlnk
    Set oShlnk = Wsh.CreateShortcut(strlnk)
If Instr(oShLnk.TargetPath,oldpath) > 0 Then
        oShLnk.TargetPath = Replace(oShLnk.TargetPath,oldpath,newpath)
        oShLnk.Save
        ji_2 = ji_2 + 1
End If
    Set oShLnk=NoThing
End Sub

Function Ask(strAction)
    Dim intButton
    intButton = MsgBox(strAction,vbQuestion + vbYesNo,"询问")
    Ask = intButton = vbYes
End Function