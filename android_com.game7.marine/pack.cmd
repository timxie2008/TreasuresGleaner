@echo off
if @%1 == @ goto help
if @%2 == @ goto help

if @%3 == @ goto default
set REMOVEALL=1
goto begin
:default
set REMOVEALL=0
:begin

setlocal enabledelayedexpansion
rem build native first
rem call build_native.cmd

rem echo replacing resources
rem copy ..\Material\ui\ui_haidi.png assets\ui\ui.png /Y > nul
rem copy ..\Material\ui\ui_haidi.plist assets\ui\ui.plist /Y > nul

rem pack every channel 

set APPNAME=com.game7.marine
set PUBLISH_PATH=..\publish\%APPNAME%
echo APPNAME=%APPNAME%
echo PUBLISH_PATH=%PUBLISH_PATH%

for /D %%I in (..\publish\com.game7.marine\*) do (
	echo preparing to build channel %%~nI 
	if exist %PUBLISH_PATH%\%%~nI\haidip_%%~nI_%1.apk (
		if %REMOVEALL% EQU 1 (
			del %PUBLISH_PATH%\%%~nI\haidip_%%~nI_%1.apk
		) else (
			echo skip %PUBLISH_PATH%\%%~nI\haidip_%%~nI_%1.apk
		)
	)
	if not exist %PUBLISH_PATH%\%%~nI\haidip_%%~nI_%1.apk (
		echo rebuilding %PUBLISH_PATH%\%%~nI\haidip_%%~nI_%1.apk

		rem update channel name
		echo coping AndroidManifest.xml file
		copy AndroidManifest.xml.template AndroidManifest.xml /Y
		echo sed AndroidManifest.xml 
		sed -i "s/__UC__/%%~nI/g" AndroidManifest.xml
		sed -i "s/__VERSIONNAME__/%1/g" AndroidManifest.xml
		sed -i "s/__VERSIONCODE__/%2/g" AndroidManifest.xml
		rem ant build
		rm -rf bin
		ant release

		echo export apk file:%PUBLISH_PATH%\%%~nI\%APPNAME%_%%~nI_%1.apk
		dir .\bin\%APPNAME%-release.apk
		rem for %%f in (.\bin\com.crazyamber.*.apk) do (
		copy .\bin\%APPNAME%-release.apk %PUBLISH_PATH%\%%~nI\%APPNAME%_%%~nI_%1.apk /Y
		rem )
		rem pause
	)
)
goto exit

:help
echo pack vername(aa.bb.cc) versioncode(aabbcc) [rebuild?]

:exit
