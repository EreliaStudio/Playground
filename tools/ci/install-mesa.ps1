[CmdletBinding()]
param([Parameter(Mandatory)][string]$Destination)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

# Match the known software renderer used by the Sparkle Version0.1.1 tests.
$version = '23.3.4'
$temporaryRoot = if ($env:RUNNER_TEMP) { $env:RUNNER_TEMP } else { [System.IO.Path]::GetTempPath() }
$archive = Join-Path $temporaryRoot "mesa3d-$version-release-msvc.7z"
$root = Join-Path $temporaryRoot "mesa-$version"

Invoke-WebRequest "https://github.com/pal1000/mesa-dist-win/releases/download/$version/mesa3d-$version-release-msvc.7z" -OutFile $archive
& 7z x $archive "-o$root" -y | Out-Null
if ($LASTEXITCODE -ne 0) { throw 'Mesa extraction failed' }

New-Item -ItemType Directory -Force $Destination | Out-Null
Copy-Item "$root/x64/*.dll" $Destination -Force
if (-not (Test-Path "$Destination/opengl32.dll")) { throw 'Mesa OpenGL DLL is missing' }
