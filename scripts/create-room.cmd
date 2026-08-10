@echo off
start "Bejeweled Server" /D "%~dp0" "%~dp0Bejeweled_Server.exe"
timeout /t 1 /nobreak >nul
start "Bejeweled Client" /D "%~dp0" "%~dp0Bejeweled_Client.exe" --host 127.0.0.1 --port 12345
