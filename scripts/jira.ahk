CheckAndGo(str)
{
  ;trim
  str = %str%
  StringLower, str, str

  if(RegExMatch(str, "cc(\d{3})", JiraID) > 0
    or RegExMatch(str, "cc-(\d{3})", JiraID) > 0)
  {
    run, http://jira.curvedms.org/browse/CC-%JiraID1%
  }
  else if(RegExMatch(str, "hero(\d{5})", JiraID) > 0
    or RegExMatch(str, "hero-(\d{5})", JiraID) > 0)
  {
    run, http://jira.curvedms.org/browse/HERO-%JiraID1%
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