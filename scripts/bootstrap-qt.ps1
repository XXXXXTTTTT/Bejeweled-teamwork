[CmdletBinding()]
param(
    [string]$QtRoot = 'D:\QT'
)

$ErrorActionPreference = 'Stop'
$qtVersion = '6.7.2'
$qtArchitecture = 'win64_mingw'
$compilerTool = 'qt.tools.win64_mingw1310'

if (-not (Get-Command python -ErrorAction SilentlyContinue)) {
    throw '未找到 Python，无法安装 aqtinstall。请先安装 Python 3。'
}

python -m pip install --user aqtinstall

python -m aqt install-qt windows desktop $qtVersion $qtArchitecture --outputdir $QtRoot
python -m aqt install-tool windows desktop tools_mingw1310 $compilerTool --outputdir $QtRoot

$projectRoot = Split-Path -Parent $PSScriptRoot
& (Join-Path $projectRoot 'tests\toolchain_check_test.ps1')
