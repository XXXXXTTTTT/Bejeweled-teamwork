$ErrorActionPreference = 'Stop'

$qtRoot = 'D:\QT\6.7.2\mingw_64'
$requiredTools = @(
    (Join-Path $qtRoot 'bin\qmake.exe'),
    (Join-Path $qtRoot 'bin\windeployqt.exe')
)

$missingTools = $requiredTools | Where-Object { -not (Test-Path -LiteralPath $_ -PathType Leaf) }
if ($missingTools) {
    throw "Qt 6.7.2 MinGW 64 位工具链不完整: $($missingTools -join ', ')"
}

$mingwMake = Get-ChildItem -LiteralPath 'D:\QT' -Filter 'mingw32-make.exe' -Recurse -File -ErrorAction SilentlyContinue | Select-Object -First 1
if ($null -eq $mingwMake) {
    throw '未找到 Qt 配套的 MinGW 13.1.0 mingw32-make.exe。'
}

Write-Host 'Qt 6.7.2 MinGW 64 位工具链检查通过。'
