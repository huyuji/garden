Option Explicit
Dim oldpath,newpath

''''''''''''''''˵��'''''''''''''''''
'���ã������޸Ŀ�ݷ�ʽ�ġ�Ŀ�ꡱ�͡���ʼλ�á�
'ʹ�÷���������Ҫ�޸ĵĿ�ݷ�ʽ����ͬһ���ļ���ִ��
'''''''''''''''˵����'''''''''''''''''

oldpath = "sforge.sspg.lab.emc.com"           '����ԭ·���н����滻������
newpath = "teamforge6.usd.lab.emc.com"   '������·����Ҫʹ�õ�����

Dim Wsh,fso
Set Wsh = WScript.CreateObject("WScript.Shell")
Set fso = CreateObject("Scripting.FileSystemObject")
Dim ji_1,ji_2,Folder
ji_1 = 0
ji_2 = 0
Folder = Wsh.CurrentDirectory
if Ask("��Ҫ�޸�"&chr(34)& Folder &chr(34)&"������п�ݷ����Ƿ����") then
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
msgbox "�ҵ� "&ji_1&" ����ݷ�ʽ"&vbCrLf&"�޸� "&ji_2&" ����ݷ�ʽ",64,"ִ�����"
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
    intButton = MsgBox(strAction,vbQuestion + vbYesNo,"ѯ��")
    Ask = intButton = vbYes
End Function