# Sanity check for m57_claude documentation update
# Run from: borne-m57-firmware root
# Checks: file presence, #pragma once, rules.mk tabs, brace balance, duplicate defines

Set-Location "C:\Users\KSCHULZ1\Code\borne-m57-firmware"

$errors = 0

# --- File presence ---
Write-Host "=== File presence ==="
$files = @(
    "source\m57_claude\config.h",
    "source\m57_claude\m57.h",
    "source\m57_claude\m57.c",
    "source\m57_claude\mcuconf.h",
    "source\m57_claude\halconf.h",
    "source\m57_claude\leftkb\config.h",
    "source\m57_claude\rightkb\config.h",
    "source\m57_claude\leftkb\mcuconf.h",
    "source\m57_claude\rightkb\mcuconf.h",
    "source\m57_claude\keymaps\via\rules.mk",
    "source\m57_claude\keymaps\via\config.h"
)
foreach ($f in $files) {
    if (Test-Path $f) { Write-Host "  OK  $f" }
    else { Write-Host "  MISSING  $f"; $errors++ }
}

# --- #pragma once in every .h ---
Write-Host "`n=== #pragma once ==="
$hFiles = $files | Where-Object { $_ -match '\.h$' }
foreach ($f in $hFiles) {
    $content = Get-Content $f -Raw
    if ($content -match '#pragma once') { Write-Host "  OK  $f" }
    else { Write-Host "  MISSING pragma once  $f"; $errors++ }
}

# --- rules.mk: no accidental tab-indented lines (would be misread as recipe) ---
Write-Host "`n=== rules.mk: no tab-indented lines ==="
$rules = Get-Content "source\m57_claude\keymaps\via\rules.mk"
$bad = $rules | Where-Object { $_ -match '^\t' }
if ($bad) {
    Write-Host "  WARNING: tab-indented lines found:"
    $bad | ForEach-Object { Write-Host "    $_" }
    $errors++
} else { Write-Host "  OK" }

# --- m57.h: brace balance in LAYOUT macro ---
Write-Host "`n=== m57.h: brace balance ==="
$mh = Get-Content "source\m57_claude\m57.h" -Raw
$open  = ([regex]::Matches($mh, '\{')).Count
$close = ([regex]::Matches($mh, '\}')).Count
if ($open -eq $close) { Write-Host "  OK  open=$open close=$close" }
else { Write-Host "  MISMATCH  open=$open close=$close"; $errors++ }

# --- config.h (root): no duplicate #define names ---
Write-Host "`n=== config.h (root): duplicate #define check ==="
$cfg = Get-Content "source\m57_claude\config.h"
$defines = $cfg | Where-Object { $_ -match '^\s*#define\s+\w+' } |
           ForEach-Object { if ($_ -match '^\s*#define\s+(\w+)') { $matches[1] } }
$dups = $defines | Group-Object | Where-Object { $_.Count -gt 1 }
if ($dups) { $dups | ForEach-Object { Write-Host "  DUPLICATE  $($_.Name)"; $errors++ } }
else { Write-Host "  OK  no duplicate defines" }

# --- Summary ---
Write-Host "`n=== Result ==="
if ($errors -eq 0) { Write-Host "  ALL CHECKS PASSED" }
else { Write-Host "  $errors ERROR(S) FOUND" }
