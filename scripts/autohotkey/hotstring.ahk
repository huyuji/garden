:*:]d::  ; This hotstring replaces "]d" with the current date and time via the commands below.
FormatTime, CurrentDate,, yyyy-MM-dd
SendInput %CurrentDate%
return

:*:]t::  ; This hotstring replaces "]d" with the current date and time via the commands below.
FormatTime, CurrentTime,, HH:mm
SendInput %CurrentTime%
return