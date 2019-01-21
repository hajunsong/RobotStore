@echo off
start D:\Qt\5.12.0\msvc2017_64\bin\windeployqt MasterServer.exe
echo MasterServer deploy completed.
start D:\Qt\5.12.0\msvc2017_64\bin\windeployqt MobileRobot.exe
echo MobileRobot deploy completed.
start D:\Qt\5.12.0\msvc2017_64\bin\windeployqt Manipulator.exe
echo Manipulator deploy completed.
start D:\Qt\5.12.0\msvc2017_64\bin\windeployqt UIMonitor.exe
echo UIMonitor deploy completed.
#pause