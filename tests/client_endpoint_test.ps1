$ErrorActionPreference = 'Stop'

$mainSource = Get-Content -LiteralPath (Join-Path $PSScriptRoot '..\Bejeweled_Client\main.cpp') -Raw
$threadSource = Get-Content -LiteralPath (Join-Path $PSScriptRoot '..\Bejeweled_Client\clientthread.cpp') -Raw

foreach ($expectedText in @('QCommandLineParser', 'host', 'port', 'ClientThread::configure')) {
    if (-not $mainSource.Contains($expectedText)) {
        throw "客户端入口未实现预期的运行时端点配置: $expectedText"
    }
}

if (-not $threadSource.Contains('127.0.0.1')) {
    throw '客户端缺少本机联机默认地址 127.0.0.1。'
}

Write-Host '客户端端点配置检查通过。'
