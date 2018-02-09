for each ps in getobject("winmgmts:\\.\root\cimv2:win32_process").instances_
if ps.name="explorer.exe" then
ps.terminate
end if 
Next

Wscript.Sleep 3000 

strComputer = "." 
Set objWMIService = GetObject("winmgmts:\\" & strComputer & "\root\cimv2") 
Set objShell = CreateObject("Wscript.Shell") 
Set colProcesses = objWMIService.ExecQuery("Select * from Win32_Process Where Name = 'explorer.exe'") 
If colProcesses.Count = 0 Then 
objShell.Run "explorer"
End If

