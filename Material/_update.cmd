md ..\Resources\env 
md ..\Resources\ui
md ..\Resources\characters
md ..\Resources\items
md ..\Resources\audio
 
copy .\env\env.plist ..\Resources\env\ /Y
copy .\env\env.png ..\Resources\env\ /Y

copy .\env\env_back.plist ..\Resources\env\ /Y
copy .\env\env_back.png ..\Resources\env\ /Y

copy .\ui\ui.plist ..\Resources\ui\ /Y
copy .\ui\ui.png ..\Resources\ui\ /Y

copy .\items\items.plist ..\Resources\items\ /Y
copy .\items\items.png ..\Resources\items\ /Y

copy .\characters\riders.plist ..\Resources\characters\ /Y
copy .\characters\riders.png ..\Resources\characters\ /Y

copy .\characters\player.plist ..\Resources\characters\ /Y
copy .\characters\player.png ..\Resources\characters\ /Y

copy .\audio\* ..\Resources\audio\ /Y

pause
