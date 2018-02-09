@echo off
setlocal enabledelayedexpansion

if {%1}=={} (
    echo Usage: %~nx0 [vhd] [option] [letter]
    exit /b 1
)
set vhdPath=%~dpnx1
set option=%2
set driveLetter=%3

if {!driveLetter!}=={} (
    echo Mounting !vhdPath!
) else (
    echo Mounting !vhdPath! to !driveLetter!:
)

REM
REM create dispart script
REM
set diskPartScript=%~nx0.diskpart
echo select vdisk file="!vhdPath!">!diskPartScript!
echo attach vdisk !option!>>!diskPartScript!

REM assign the drive letter if requested
if not {!driveLetter!}=={} (
    echo select partition 1 >>!diskPartScript!
    echo assign letter=!driveLetter!>>!diskPartScript!
)

REM Show script
echo.
echo Running diskpart script:
type !diskPartScript!

REM
REM diskpart
REM
diskpart /s !diskPartScript!
del /q !diskPartScript!

echo Done!

endlocal