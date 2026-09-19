[CmdletBinding()]
param([Parameter(Mandatory)][string]$Destination)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

# This is the single pin updated when Playground adopts a new Sparkle package.
$version = '0.1.1'
$revision = '68c260aa2108cd22c26ba09a2c8ca6f62913ad39'
$sha256 = '337502f93d22c45f742e5b2a39c472a6068a6b131deebc028f610a6c118d79e2'
$packageName = "sparkle-$version-g$revision-windows-x64-clangcl-static-release"
$releaseTag = "sparkle-v$version-g$revision"
$url = "https://github.com/EreliaStudio/Sparkle/releases/download/$releaseTag/$packageName.zip"

$destinationPath = [System.IO.Path]::GetFullPath($Destination)
$temporaryRoot = if ($env:RUNNER_TEMP) { $env:RUNNER_TEMP } else { [System.IO.Path]::GetTempPath() }
$archive = Join-Path $temporaryRoot "$packageName.zip"

Remove-Item $destinationPath -Recurse -Force -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force $destinationPath | Out-Null
Invoke-WebRequest $url -OutFile $archive

$actualSha256 = (Get-FileHash $archive -Algorithm SHA256).Hash.ToLowerInvariant()
if ($actualSha256 -ne $sha256) {
    throw "Sparkle package checksum mismatch: expected $sha256, received $actualSha256"
}

Expand-Archive $archive $destinationPath -Force
$prefix = Join-Path $destinationPath $packageName
$config = Join-Path $prefix 'lib/cmake/sparkle/sparkleConfig.cmake'
if (-not (Test-Path $config)) { throw "Sparkle package config is missing: $config" }

if ($env:GITHUB_ENV) { "SPARKLE_PREFIX=$prefix" | Out-File $env:GITHUB_ENV -Append }
Write-Host "Installed Sparkle $version ($revision) at $prefix"
Write-Output $prefix
