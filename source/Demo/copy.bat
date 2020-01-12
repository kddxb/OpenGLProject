::echo %cd%
::echo %~dp0

set ROOTDIR=..\..\
set PLATFORM=%~1
set CONFIGURATION=%~2

xcopy /e /y config.json %ROOTDIR%bin\%PLATFORM%%CONFIGURATION%\