:*:]d::  ; This hotstring replaces "]d" with the current date and time via the commands below.
FormatTime, CurrentDateTime,, yyyy-MM-dd ; It will look like 9/1/2005 3:53 PM
SendInput %CurrentDateTime%
return