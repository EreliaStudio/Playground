[CmdletBinding()]
param(
    [Parameter(Mandatory)][string]$BuildDirectory,
    [Parameter(Mandatory)][string]$Prefix,
    [Parameter(Mandatory)][string]$ConfigureLog
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$build = (Resolve-Path $BuildDirectory).Path
$prefixPath = (Resolve-Path $Prefix).Path
$cache = Join-Path $build 'CMakeCache.txt'
$log = (Resolve-Path $ConfigureLog).Path
if (-not (Test-Path $cache)) { throw "CMake cache is missing: $cache" }

$escapedPackageDirectory = [regex]::Escape((Join-Path $prefixPath 'lib/cmake/sparkle').Replace('\', '/'))
$normalizedCache = (Get-Content $cache -Raw).Replace('\', '/')
if ($normalizedCache -notmatch "(?m)^sparkle_DIR:PATH=$escapedPackageDirectory$") {
    throw 'CMake did not resolve sparkle_DIR from the downloaded package prefix'
}

$configureOutput = Get-Content $log -Raw
if ($configureOutput -match '(?im)Building package sparkle|Building sparkle|sparkle(?:\[[^]]+\])?:x64-windows') {
    throw 'Playground configure attempted to build Sparkle through vcpkg'
}

$installedSparkle = Get-ChildItem $build -Directory -Recurse -ErrorAction SilentlyContinue |
    Where-Object { $_.FullName -match '[\\/]vcpkg_installed[\\/].*[\\/]share[\\/]sparkle$' }
if ($installedSparkle) { throw 'Sparkle was installed into Playground vcpkg_installed' }

$cmakeFiles = Get-ChildItem "$prefixPath/lib/cmake/sparkle" -File -Filter '*.cmake'
if ($cmakeFiles | Select-String -Pattern '(?i)[A-Z]:[\\/].*(?:sparkle-build|prebuilt-package[\\/]source)' -Quiet) {
    throw 'The installed Sparkle CMake metadata embeds its packaging source/build directory'
}

if ($env:GITHUB_WORKSPACE -and (Test-Path (Join-Path $env:GITHUB_WORKSPACE 'Sparkle'))) {
    throw 'A Sparkle source checkout exists in the Playground CI workspace'
}

Write-Host 'Verified that Playground consumes only the downloaded Sparkle package.'
