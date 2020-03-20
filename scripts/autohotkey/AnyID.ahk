CheckAndGo(str)
{
  ;trim
  str = %str%

  if(RegExMatch(str, "C(\d{7})", ListingID) > 0)
  {
    run, https://teamforge6.usd.lab.emc.com/sf/go/%TeamforgeID%
  }
  else if(RegExMatch(Clipboard,"(US|us)\d+",RallyAR) > 0 
          or RegExMatch(Clipboard,"(TA|ta)\d+",RallyAR) > 0
          or RegExMatch(Clipboard,"(F|f)\d+",RallyAR) > 0)
  {
    run,https://rally1.rallydev.com/#/search?keywords=%RallyAR%
  }
  else if(RegExMatch(str, "^\d{6}$", AR) > 0)
  {
    run, https://arswebprd01.isus.emc.com:8443/arsys/forms/arsappprd02.isus.emc.com/EMC`%3AIssue+Tracking/Default/?eid=000000000%AR%
  }
  else if(RegExMatch(str, "^\d{5}$", ReviewRequestID) > 0)
  {
    run,http://reviewforge.clrcase.lab.emc.com/r/%ReviewRequestID%
  }
  else
  {
    return 0
  }
  
  return 1
}

#z::
#SingleInstance force
SendMode Input

;save current clipboard content
OrgClipboard := Clipboard

;use ctrl+c to copy
Clipboard=
Send, {CTRLDOWN}c{CTRLUP}
ClipWait, 2

if ErrorLevel
{
  MsgBox, The attempt to copy text onto the clipboard failed.
}
else
{
  CheckAndGo(Clipboard)
}

;Restore clipboard
Clipboard :=OrgClipboard
ClipWait, 2