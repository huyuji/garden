Function PathRemoveFileSpec4(strFileName)
  Dim fso,file
  Set fso = WSH.CreateObject("Scripting.FileSystemObject")
    If fso.FileExists(strFileName) Then
      Set file = fso.GetFile(strFileName)
      PathRemoveFileSpec4 = fso.GetParentFolderName(file)
      Set file = Nothing
    ElseIf fso.FolderExists(strFileName) Then
      ' 如果已经是文件夹则不需要处理直接返回
      PathRemoveFileSpec4 = strFileName
    Else
      ' Oops 文件或文件夹不存在
      ' 原样返回当然也可以调用
      ' PathRemoveFileSpec2正则文本处理一下
      PathRemoveFileSpec4 = strFileName
    End If
  Set fso = Nothing
End Function

'
' Description: VBScript/VBS open file dialog
'              Compatible with most Windows platforms
' Author: wangye  <pcn88 at hotmail dot com>
' Website: http://wangye.org
'
' dir is the initial directory; if no directory is
' specified "Desktop" is used.
' filter is the file type filter; format "File type description|*.ext"
'
Public Function GetOpenFileName(dir, filter)
    Const msoFileDialogFilePicker = 3
 
    If VarType(dir) <> vbString Or dir="" Then
        dir = CreateObject( "WScript.Shell" ).SpecialFolders( "Desktop" )
    End If
 
    If VarType(filter) <> vbString Or filter="" Then
        filter = "All files|*.*"
    End If
 
    Dim i,j, objDialog, TryObjectNames
    TryObjectNames = Array( _
        "UserAccounts.CommonDialog", _
        "MSComDlg.CommonDialog", _
        "MSComDlg.CommonDialog.1", _
        "Word.Application", _
        "SAFRCFileDlg.FileOpen", _
        "InternetExplorer.Application" _
        )
 
    On Error Resume Next
    Err.Clear
 
    For i=0 To UBound(TryObjectNames)
        Set objDialog = WSH.CreateObject(TryObjectNames(i))
        If Err.Number<>0 Then
        Err.Clear
        Else
        Exit For
        End If
    Next
 
    Select Case i
        Case 0,1,2
        ' 0. UserAccounts.CommonDialog XP Only.
        ' 1.2. MSComDlg.CommonDialog MSCOMDLG32.OCX must registered.
        If i=0 Then
            objDialog.InitialDir = dir
        Else
            objDialog.InitDir = dir
        End If
        objDialog.Filter = filter
        If objDialog.ShowOpen Then
            GetOpenFileName = objDialog.FileName
        End If
        Case 3
        ' 3. Word.Application Microsoft Office must installed.
        objDialog.Visible = False
        Dim objOpenDialog, filtersInArray
        filtersInArray = Split(filter, "|")
        Set objOpenDialog = _
            objDialog.Application.FileDialog( _
                msoFileDialogFilePicker)
            With objOpenDialog
            .Title = "Open File(s):"
            .AllowMultiSelect = False
            .InitialFileName = dir
            .Filters.Clear
            For j=0 To UBound(filtersInArray) Step 2
                .Filters.Add filtersInArray(j), _
                     filtersInArray(j+1), 1
            Next
            If .Show And .SelectedItems.Count>0 Then
                GetOpenFileName = .SelectedItems(1)
            End If
            End With
            objDialog.Visible = True
            objDialog.Quit
        Set objOpenDialog = Nothing
        Case 4
        ' 4. SAFRCFileDlg.FileOpen xp 2003 only
        ' See http://www.robvanderwoude.com/vbstech_ui_fileopen.php
        If objDialog.OpenFileOpenDlg Then
           GetOpenFileName = objDialog.FileName
        End If
        Case 5
        ' 5. InternetExplorer.Application IE must installed
        objDialog.Navigate "about:blank"
        Dim objBody, objFileDialog
        Set objBody = _
            objDialog.document.getElementsByTagName("body")(0)
        objBody.innerHTML = "<input type='file' id='fileDialog'>"
        while objDialog.Busy Or objDialog.ReadyState <> 4
            WScript.sleep 10
        Wend
        Set objFileDialog = objDialog.document.all.fileDialog
            objFileDialog.click
            GetOpenFileName = objFileDialog.value
            objDialog.Quit
        Set objFileDialog = Nothing
        Set objBody = Nothing
        Case Else
        ' Sorry I cannot do that!
    End Select
 
    Set objDialog = Nothing
End Function
 
Dim strFileName
strFileName = GetOpenFileName(PathRemoveFileSpec4(WScript.ScriptFullName),"All files|*.*|Microsoft Excel|*.xls, *.xlsx")

dim fso
Set fso = WSH.CreateObject("Scripting.FileSystemObject")
If not fso.FileExists(strFileName) Then
  MsgBox "No file selected"
  Wscript.Quit
End if

dim oExcel,oWb,oSheet
Set oExcel= CreateObject("Excel.Application")
Set oWb = oExcel.Workbooks.Open(strFileName)
Set oSheet = oWb.Sheets("UK Consol-Combine")

dim dstCol1
dstCol1 = "D"
dim srcCol1
srcCol1 = "L"
dim dstCol2
dstCol2 = "W"
dim srcCol2
srcCol2 = "AC"

dim zeroCol1
zeroCol1 = "F"
dim zeroCol2
zeroCol2 = "Y"

For Row = 5 to 53
  if StrComp(Left(oSheet.Range(dstCol1 & Row).Formula & "", 2), "=D") then
    oSheet.Range(dstCol1 & Row).Value = oSheet.Range(srcCol1 & Row).Value
    oSheet.Range(dstCol2 & Row).Value = oSheet.Range(srcCol2 & Row).Value
    
    oSheet.Range(srcCol1 & Row).Value = 0
    oSheet.Range(srcCol2 & Row).Value = 0
    oSheet.Range(zeroCol1 & Row).Value = 0
    oSheet.Range(zeroCol2 & Row).Value = 0
  end if
Next

oWb.Save
oWb.Close
oExcel.Quit

Set oWb = nothing
Set oExcel = nothing

MsgBox "Complete!"