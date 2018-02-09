'determine destination file name
if (Wscript.Arguments.Count <> 1) then
	wscript.echo("Daily ARS Status Download - no folder provided")
	wscript.quit
end if

dim folder
folder = Wscript.Arguments(0)

'check folder existence
set fso = CreateObject("Scripting.FileSystemObject")
if (fso.FolderExists(folder) = false) then
	wscript.echo("Daily ARS Status Download - folder not exist")
	wscript.quit
end if

'make sure folder path ends with "\"
if(Right(folder, 1) <> "\") then
	folder = folder + "\"
end if

filename = folder + "Daily ARS Status " + Cstr(Date()) + ".html"
'wscript.echo(filename)

'get the webpage
set xmlServerHttp = CreateObject( "Msxml2.ServerXMLHTTP")
xmlServerHttp.setTimeouts 1000*60*30, 1000*60*30, 1000*60*30, 1000*60*30
xmlServerHttp.open "GET", "http://www.cae.lab.emc.com/cgi-bin/caears.pl", False
xmlServerHttp.send

'write the content of the webpage into destination file
Set WriteStuff = fso.OpenTextFile(filename, 8, True) ' "8" means append, "true" means create new
WriteStuff.Write(xmlServerHttp.responseText)
WriteStuff.Close
SET WriteStuff = NOTHING
SET fso = NOTHING

Set xmlServerHttp = Nothing 

Wscript.echo("finished!")