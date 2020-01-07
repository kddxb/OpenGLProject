::echo %cd%
::echo %~dp0

set ROOTDIR=..\
set PLATFORM=%~1
set CONFIGURATION=%~2

IF %CONFIGURATION%==Release xcopy /e /y %ROOTDIR%thirdparty\dll\%PLATFORM%\*.* %ROOTDIR%bin\%PLATFORM%%CONFIGURATION%\