@echo off
start C:\Qt\5.11.2\msvc2017_64\bin\windeployqt MasterServer.exe
echo MasterServer deploy completed.
start C:\Qt\5.11.2\msvc2017_64\bin\windeployqt MobileRobot.exe
echo MobileRobot deploy completed.
start C:\Qt\5.11.2\msvc2017_64\bin\windeployqt Manipulator.exe
echo Manipulator deploy completed.
start C:\Qt\5.11.2\msvc2017_64\bin\windeployqt UIMonitor.exe
echo UIMonitor deploy completed.
#pause