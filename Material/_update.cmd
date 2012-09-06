md ..\Resources\env 
md ..\Resources\ui
md ..\Resources\character
 
copy .\env\env.plist ..\Resources\env\ /Y
copy .\env\env.png ..\Resources\env\ /Y

copy .\ui\ui.plist ..\Resources\ui\ /Y
copy .\ui\ui.png ..\Resources\ui\ /Y

copy .\dolphin\dolphin.plist ..\Resources\character\ /Y
copy .\dolphin\dolphin.png ..\Resources\character\ /Y

copy .\shark\shark.plist ..\Resources\character\ /Y
copy .\shark\shark.png ..\Resources\character\ /Y

copy .\player\player.plist ..\Resources\character\ /Y
copy .\player\player.png ..\Resources\character\ /Y

pause
