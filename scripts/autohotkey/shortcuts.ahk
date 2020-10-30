#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

; win + alt + down to minimize window 
#!Down::WinMinimize, A


; win + alt + o to open outlook
#!O::Run, "C:\Program Files\Microsoft Office\root\Office16\OUTLOOK.EXE" /recycle

; win + alt + t to open teams
#!T::Run, "C:\Users\yuji.hu\AppData\Local\Microsoft\Teams\current\Teams.exe" /recycle

; win + alt + t to open freecommander
#!F::Run, C:\Program Files (x86)\FreeCommander XE\FreeCommander.exe