:*:]d::  ; This hotstring replaces "]d" with the current date and time via the commands below.
FormatTime, CurrentDate,, yyyy-MM-dd
SendInput %CurrentDate%
return

:*:]t::  ; This hotstring replaces "]t" with the current date and time via the commands below.
FormatTime, CurrentTime,, HH:mm:ss
SendInput %CurrentTime%
return