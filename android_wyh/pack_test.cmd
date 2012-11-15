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

set APPNAME=com.wyh.ttlus
set PUBLISH_PATH=..\publish\%APPNAME%
echo APPNAME=%APPNAME%
echo PUBLISH_PATH=%PUBLISH_PATH%

for /D %%I in (..\publish\com.wyh.ttlus\*) do (
  echo export apk file:%PUBLISH_PATH%\
  echo export apk file:%PUBLISH_PATH%\%%~nI\
  echo export apk file:%PUBLISH_PATH%\%%~nI\%APPNAME%_%%~nI_%1.apk
)
goto exit

:help
echo pack vername(aa.bb.cc) versioncode(aabbcc) [rebuild?]

:exit
