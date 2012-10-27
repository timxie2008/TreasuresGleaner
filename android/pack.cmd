@echo off
if @%1 == @ goto help
if @%2 == @ goto help

setlocal enabledelayedexpansion
rem build native first
call build_native.cmd

rem pack every channel 
for /D %%i in (../publish/channels/*) do (
	if exist ..\publish\channels\%%i\ttlus_%%i_%1.apk (
		echo skip ..\publish\channels\%%i\ttlus_%%i_%1.apk
	)
	if not exist ..\publish\channels\%%i\ttlus_%%i_%1.apk (
		rem now %%i is a channel name
		rem update channel name
		copy AndroidManifest.xml.template AndroidManifest.xml /Y
		sed -i "s/__UC__/%%i/g" AndroidManifest.xml
		sed -i "s/__VERSIONNAME__/%1/g" AndroidManifest.xml
		sed -i "s/__VERSIONCODE__/%2/g" AndroidManifest.xml

		rem ant build
		rm -rf bin
		ant release

		echo export apk file:..\publish\channels\%%i\ttlus_%%i_%1.apk
		rem dir .\bin\com.crazyamber.ttlus-release.apk
		rem for %%f in (.\bin\com.crazyamber.*.apk) do (
		copy .\bin\com.crazyamber.ttlus-release.apk ..\publish\channels\%%i\ttlus_%%i_%1.apk /Y
		rem )
		rem pause
	)
)
goto exit

:help
echo pack vername(aa.bb.cc) versioncode(aabbcc)

:exit
