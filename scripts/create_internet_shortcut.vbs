for i = 0 to WScript.Arguments.Count - 1
  if(StrComp(LCase(WScript.Arguments.Item(i)), "-shortcut")=0) Then
	shortcut = WScript.Arguments.Item(i + 1)
  elseif(StrComp(LCase(WScript.Arguments.Item(i)), "-link") = 0 ) Then
	  link = WScript.Arguments.Item(i + 1)
  end if
next

Set objShell = CreateObject("Wscript.Shell")
Set objShortcut = objShell.CreateShortcut(shortcut)
objShortcut.TargetPath = link
objShortcut.Save