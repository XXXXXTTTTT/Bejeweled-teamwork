[CmdletBinding()]
param([string]$Output = 'dist\portable')
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$outputPath = [IO.Path]::GetFullPath((Join-Path $root $Output))
New-Item -ItemType Directory -Force -Path $outputPath | Out-Null
Copy-Item (Join-Path $root 'build\client-release\release\Bejeweled_Client.exe') $outputPath -Force
Copy-Item (Join-Path $root 'build\server-release\release\Bejeweled_Server.exe') $outputPath -Force
Copy-Item (Join-Path $root 'Bejeweled_Server\resources') (Join-Path $outputPath 'resources') -Recurse -Force
Set-Content -LiteralPath (Join-Path $outputPath 'Start_Host.cmd') -Encoding ascii -Value @(
    '@echo off',
    'start "Bejeweled Server" /D "%~dp0" "%~dp0Bejeweled_Server.exe"',
    'timeout /t 1 /nobreak >nul',
    'start "Bejeweled Client" /D "%~dp0" "%~dp0Bejeweled_Client.exe" --host 127.0.0.1 --port 12345'
)
Set-Content -LiteralPath (Join-Path $outputPath 'Join_Room.cmd') -Encoding ascii -Value @(
    '@echo off',
    'set /p HOST=Server IPv4 address: ',
    'start "Bejeweled Client" /D "%~dp0" "%~dp0Bejeweled_Client.exe" --host %HOST% --port 12345'
)
& 'D:\QT\6.7.2\mingw_64\bin\windeployqt.exe' --release --compiler-runtime --multimedia --sql (Join-Path $outputPath 'Bejeweled_Client.exe')
& 'D:\QT\6.7.2\mingw_64\bin\windeployqt.exe' --release --compiler-runtime --sql (Join-Path $outputPath 'Bejeweled_Server.exe')
$zip = Join-Path (Split-Path $outputPath) 'Bejeweled-Windows-x64-portable.zip'
if (Test-Path $zip) { Remove-Item $zip -Force }
Compress-Archive -Path (Join-Path $outputPath '*') -DestinationPath $zip
Write-Host "已生成 $zip"
