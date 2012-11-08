rem @echo off
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

echo replacing resources
copy ..\Material\ui\ui_haidi.png assets\ui\ui.png /Y
copy ..\Material\ui\ui_haidi.plist assets\ui\ui.plist /Y

rem pack every channel 

for /D %%i in (../publish/pushversion/*) do (
	echo preparing to build channel %%i 
	if exist ..\publish\pushversion\%%i\haidip_%%i_%1.apk (
		echo checking
		if %REMOVEALL% EQU 1 (
			set REBUILD=1
		) else (
			set REBUILD=0
			echo FUCK!!! chk gf=%REMOVEALL% rb=%REBUILD% now REBUILD is 1!!!!!
			echo skip ..\publish\pushversion\%%i\haidip_%%i_%1.apk
		)
	) else (
		set REBUILD=1
	)
	echo gf=%REMOVEALL% rb=%REBUILD%
	pause
	
	if %REBUILD% EQU 1 (
		echo rebuilding ..\publish\pushversion\%%i\haidip_%%i_%1.apk
		
		rem update channel name
		echo coping AndroidManifest.xml file
		copy AndroidManifest.xml.template AndroidManifest.xml /Y
		echo sed AndroidManifest.xml 
		sed -i "s/__UC__/%%i/g" AndroidManifest.xml
		sed -i "s/__VERSIONNAME__/%1/g" AndroidManifest.xml
		sed -i "s/__VERSIONCODE__/%2/g" AndroidManifest.xml
		rem ant build
		rm -rf bin
		ant release

		echo export apk file:..\publish\pushversion\%%i\haidip_%%i_%1.apk
		rem dir .\bin\com.crazyamber.haidip-release.apk
		rem for %%f in (.\bin\com.crazyamber.*.apk) do (
		copy .\bin\com.crazyamber.haidip-release.apk ..\publish\pushversion\%%i\haidip_%%i_%1.apk /Y
		rem )
		rem pause
	)
)
goto exit

:help
echo pack vername(aa.bb.cc) versioncode(aabbcc)

:exit
