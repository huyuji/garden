@echo off

set symbolPath=C:\Program Files\CircleCardiovascularImaging\cvi42Auto

set dumpPath=%1
For %%A in (%dumpPath%) do (
    Set dumpName=%%~nxA
)

cdb -c "!analyze -v;q" -y "%symbolPath%" -logo "%dumpName%.txt" -z %dumpPath%
