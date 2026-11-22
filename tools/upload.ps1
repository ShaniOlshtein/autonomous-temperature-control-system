# tools/upload.ps1 - UPLOAD. Human-run only (decisions 26, 32).
# Uploads the binary produced by tools\build.ps1; it does not compile.
# Before uploading it shows the .bin (full path, size, timestamp) and the port, reminds you to
# close any Serial Monitor, and asks you to type the port again to confirm.
#
# Human:  powershell -NoProfile -ExecutionPolicy Bypass -File tools\upload.ps1 -Port COMxx
# Agent:  add -Confirmed ONLY when the Lead's prompt says the human approved THIS upload
#         (same .bin and same port). -Confirmed skips only the typed confirmation;
#         everything else is still printed.
param(
    [Parameter(Mandatory = $true)][ValidatePattern('^COM\d+$')][string]$Port,
    [switch]$Confirmed
)

$RepoRoot  = Split-Path -Parent $PSScriptRoot
$SketchDir = Join-Path $RepoRoot 'cooking_temperature_control'   # sketch folder (.ino, .cpp, .h, sketch.yaml)
$BuildPath = Join-Path $env:LOCALAPPDATA 'arduino-build\cooking_temperature_control'
$Bin       = Join-Path $BuildPath 'cooking_temperature_control.ino.bin'
$Yaml      = Join-Path $SketchDir 'sketch.yaml'

function Fail($msg) { Write-Host "UPLOAD ABORTED: $msg"; exit 2 }

$Cli = (Get-Command arduino-cli -ErrorAction SilentlyContinue).Source
if (-not $Cli) { $Cli = 'C:\tools\arduino-cli\arduino-cli.exe' }
if (-not (Test-Path $Cli)) { Fail "arduino-cli not found (PATH or $Cli)" }
if (-not (Test-Path $Yaml)) { Fail "sketch.yaml not found" }
if (Select-String -Path $Yaml -Pattern '^\s*(default_port|default_protocol|default_profile|profiles)\s*:' -Quiet) {
    Fail "sketch.yaml contains a port or a profile"
}
if (-not (Test-Path $Bin)) { Fail "no binary at $Bin. Run tools\build.ps1 first." }
$env:LANG = 'en'

# (a) What will be flashed, and where.
$BinItem  = Get-Item $Bin
$FqbnLine = (Select-String -Path $Yaml -Pattern '^\s*default_fqbn\s*:' | Select-Object -First 1).Line.Trim()
Write-Host ""
Write-Host "================ UPLOAD ================"
Write-Host "Binary   : $($BinItem.FullName)"
Write-Host "Size     : $($BinItem.Length) bytes"
Write-Host "Built at : $($BinItem.LastWriteTime.ToString('yyyy-MM-dd HH:mm:ss'))"
Write-Host "Port     : $Port"
Write-Host "Board    : $FqbnLine"
Write-Host "========================================"
Write-Host ""

# (b) The port must be free.
Write-Host "REMINDER: close every Serial Monitor on $Port before uploading"
Write-Host "          (Arduino IDE Serial Monitor, VS Code serial monitor, arduino-cli monitor)."
Write-Host "          The upload fails if another program holds the port."
Write-Host ""

if ($Confirmed) {
    Write-Host "Confirmation: -Confirmed given (human approved this upload via the Lead)."
} else {
    $Typed = Read-Host "To flash this binary to $Port, type the port again (anything else cancels)"
    if ($null -eq $Typed -or $Typed.Trim().ToUpper() -ne $Port.ToUpper()) {
        Write-Host "Upload cancelled. Nothing was flashed."
        exit 1
    }
}

# FQBN (upload speed, flash settings, erase = none) comes from sketch.yaml default_fqbn.
$UploadArgs = @('upload', '-p', $Port, '--input-dir', $BuildPath, $SketchDir)
Write-Host "> `"$Cli`" $($UploadArgs -join ' ')"
& $Cli @UploadArgs
$Code = $LASTEXITCODE
Write-Host "exit code: $Code"
exit $Code
