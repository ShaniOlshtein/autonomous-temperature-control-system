# tools/build.ps1 - COMPILE ONLY. This script never uploads and contains no port.
# Decisions 21 (core 3.3.11), 22 (board settings via sketch.yaml), 31, 32.
# Run: powershell -NoProfile -ExecutionPolicy Bypass -File tools\build.ps1

$RequiredCore = '3.3.11'
$RepoRoot     = Split-Path -Parent $PSScriptRoot      # repo root
$SketchDir    = Join-Path $RepoRoot 'cooking_temperature_control'   # sketch folder (.ino, .cpp, .h, sketch.yaml)
$Sketchbook   = Split-Path -Parent $RepoRoot          # ...\OneDrive\Arduino (holds libraries\)
$BuildPath    = Join-Path $env:LOCALAPPDATA 'arduino-build\cooking_temperature_control'
$Yaml         = Join-Path $SketchDir 'sketch.yaml'
$RepoSetup    = Join-Path $RepoRoot 'tft_setup\Setup206_LilyGo_T_Display_S3.h'

function Fail($msg) { Write-Host "BUILD ABORTED: $msg"; exit 2 }

# arduino-cli: PATH first, then the install folder from plan step 1.
$Cli = (Get-Command arduino-cli -ErrorAction SilentlyContinue).Source
if (-not $Cli) { $Cli = 'C:\tools\arduino-cli\arduino-cli.exe' }
if (-not (Test-Path $Cli)) { Fail "arduino-cli not found (PATH or $Cli)" }

# Process-only environment. The arduino-cli config file is NOT changed.
$env:LANG = 'en'                                   # English output ("Sketch uses ...")
$env:ARDUINO_DIRECTORIES_USER = $Sketchbook        # use the IDE 1.8.19 libraries
if (-not (Test-Path (Join-Path $Sketchbook 'libraries'))) { Fail "no libraries folder in $Sketchbook" }

# Guard: sketch.yaml exists, has default_fqbn, and has no port and no profile.
if (-not (Test-Path $Yaml)) { Fail "sketch.yaml not found" }
if (Select-String -Path $Yaml -Pattern '^\s*(default_port|default_protocol|default_profile|profiles)\s*:' -Quiet) {
    Fail "sketch.yaml contains a port or a profile"
}
$FqbnLine = Select-String -Path $Yaml -Pattern '^\s*default_fqbn\s*:' | Select-Object -First 1
if (-not $FqbnLine) { Fail "sketch.yaml has no default_fqbn" }
Write-Host "sketch.yaml $($FqbnLine.Line.Trim())"

# Guard: ESP32 core must be exactly 3.3.11. This script never installs or upgrades.
$CoreJson = (& $Cli core list --json) -join "`n"
if ($LASTEXITCODE -ne 0) { Fail "arduino-cli core list failed" }
$Core = ($CoreJson | ConvertFrom-Json).platforms | Where-Object { $_.id -eq 'esp32:esp32' }
if (-not $Core) { Fail "core esp32:esp32 not installed" }
if ($Core.installed_version -ne $RequiredCore) { Fail "core esp32:esp32 is $($Core.installed_version), required $RequiredCore" }
Write-Host "core esp32:esp32 $($Core.installed_version)"

# Guard: TFT_eSPI must select Setup206, and its Setup206 must equal the repo copy.
$Select = @(Get-ChildItem -Path (Join-Path $Sketchbook 'libraries\*\User_Setup_Select.h') -ErrorAction SilentlyContinue)
if ($Select.Count -ne 1) { Fail "expected exactly one TFT_eSPI User_Setup_Select.h, found $($Select.Count)" }
$Active = @(Select-String -Path $Select[0].FullName -Pattern '^\s*#include\s*[<"]User_Setup')
if ($Active.Count -ne 1 -or $Active[0].Line -notmatch 'User_Setups/Setup206_LilyGo_T_Display_S3\.h') {
    Fail "TFT_eSPI User_Setup_Select.h does not select only Setup206 ($($Select[0].FullName))"
}
$LibSetup = Join-Path $Select[0].DirectoryName 'User_Setups\Setup206_LilyGo_T_Display_S3.h'
if ((Get-FileHash $LibSetup).Hash -ne (Get-FileHash $RepoSetup).Hash) { Fail "library Setup206 differs from tft_setup copy" }
Write-Host "TFT_eSPI setup: Setup206 (matches tft_setup copy)"

# Compile. No -b (FQBN comes from sketch.yaml), no -p, no -u.
$CompileArgs = @('compile', '--warnings', 'all', '--build-path', $BuildPath, $SketchDir)
Write-Host "> `"$Cli`" $($CompileArgs -join ' ')"
& $Cli @CompileArgs
$Code = $LASTEXITCODE
Write-Host "exit code: $Code"
exit $Code
