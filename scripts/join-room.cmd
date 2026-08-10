@echo off
set /p HOST=请输入服务端 IPv4 地址:
start "Bejeweled Client" /D "%~dp0" "%~dp0Bejeweled_Client.exe" --host %HOST% --port 12345
