@echo off
mkdir execute
copy ..\MobileRobot\Release\release\MobileRobot.exe .\execute\
copy ..\PLCServer\Release\release\PLCServer.exe .\execute\
copy ..\RobotStore\Release\release\RobotStore.exe .\execute\
C:\Qt\5.12.2\mingw73_32\bin\windeployqt .\execute\MobileRobot.exe
C:\Qt\5.12.2\mingw73_32\bin\windeployqt .\execute\PLCServer.exe
C:\Qt\5.12.2\mingw73_32\bin\windeployqt .\execute\RobotStore.exe
echo deploy completed
pause