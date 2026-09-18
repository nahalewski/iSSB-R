param([switch]$AllowIncomplete)
$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$baseline = '837ad567fcf1a969665fde3fd4057d7f18999e3d'
$spriteDir = Join-Path $repo 'Assets/Images/Game/Characters/Ichigo'
$source = Get-Content (Join-Path $repo 'Source/SmashBros/Game/Characters/Ichigo.cpp') -Raw
$pattern = 'add(?:TwoSided|Single)Animation\("([^" ]+)", "([^" ]+)", (\d+), (\d+), (\d+)\)'
$animations = @([regex]::Matches($source, $pattern))
# Winner-screen values come from CharacterLoader, not Ichigo::Load.
$loader = Get-Content (Join-Path $repo 'Source/SmashBros/Loader.cpp') -Raw
$winner = [regex]::Match($loader, 'case Global::CHAR_ICHIGO:\s*win_fps = (\d+);\s*win_rows = (\d+);\s*lose_fps = (\d+);\s*lose_rows = (\d+);')
if(!$winner.Success) { throw 'Cannot determine Ichigo winner-screen frame counts' }
$winnerDefinitions = 'addSingleAnimation("win", "win.png", '+$winner.Groups[1].Value+', '+$winner.Groups[2].Value+', 1)'+"`n"+
    'addSingleAnimation("win_hold", "win_hold.png", 1, 1, 1)'+"`n"+
    'addSingleAnimation("lose", "lose.png", '+$winner.Groups[3].Value+', '+$winner.Groups[4].Value+', 1)'
$animations += @([regex]::Matches($winnerDefinitions, $pattern))
$results = @()
foreach($anim in $animations) {
    $file = $anim.Groups[2].Value
    $path = Join-Path $spriteDir $file
    $frames = [int]$anim.Groups[4].Value * [int]$anim.Groups[5].Value
    if(!(Test-Path ($path + '.hd'))) {
        if(!$AllowIncomplete) { throw "Missing HD replacement: $file" }
        continue
    }
    $raw = [IO.File]::ReadAllBytes($path + '.hd')
    $newline = [Array]::IndexOf($raw, [byte]10)
    $header = [Text.Encoding]::ASCII.GetString($raw, 0, $newline).Split(' ')
    if($header[0] -ne 'ISSBHD1') { throw "Invalid header: $file" }
    $density = [int]$header[1]; $width = [int]$header[2]; $height = [int]$header[3]
    if($density -ne 4 -or $raw.Length -ne ($newline+1+$width*$height)) { throw "Invalid metadata length/density: $file" }
    $bitmap = [Drawing.Bitmap]::new($path)
    try {
        if($bitmap.Width -ne $width*4 -or $bitmap.Height -ne $height*4) { throw "Dimensions changed incorrectly: $file" }
        if($bitmap.GetPixel(0,0).A -ne 0) { throw "Opaque background: $file" }
        $cellHeight = [int][Math]::Floor($height/[int]$anim.Groups[4].Value)*4
        for($frame=0;$frame -lt $frames;$frame++) {
            $found = $false
            for($y=$frame*$cellHeight;$y -lt ($frame+1)*$cellHeight -and !$found;$y+=2) {
                for($x=0;$x -lt $bitmap.Width;$x+=2) {
                    if($bitmap.GetPixel($x,$y).A -gt 16) { $found=$true; break }
                }
            }
            if(!$found) { throw "Empty frame $frame in $file" }
        }
    } finally { $bitmap.Dispose() }
    # Read the original PNG directly from Git without a binary PowerShell pipeline.
    $start = [Diagnostics.ProcessStartInfo]::new()
    $start.FileName = 'rtk'
    $start.Arguments = 'proxy git -C "' + $repo + '" show ' + $baseline + ':Assets/Images/Game/Characters/Ichigo/' + $file
    $start.UseShellExecute = $false; $start.RedirectStandardOutput = $true; $start.CreateNoWindow = $true
    $process = [Diagnostics.Process]::Start($start)
    $memory = [IO.MemoryStream]::new()
    $process.StandardOutput.BaseStream.CopyTo($memory); $process.WaitForExit()
    if($process.ExitCode -ne 0) { throw "Cannot read original: $file" }
    $memory.Position=0
    $original = [Drawing.Bitmap]::new($memory)
    try {
        if($original.Width -ne $width -or $original.Height -ne $height) { throw "Logical size differs from original: $file" }
        $index=$newline+1
        for($y=0;$y -lt $height;$y++) {
            for($x=0;$x -lt $width;$x++) {
                $expected = [byte]($original.GetPixel($x,$y).A -gt 0)
                if($raw[$index] -ne $expected) { throw "Original collision mask changed: $file at $x,$y" }
                $index++
            }
        }
    } finally { $original.Dispose(); $memory.Dispose(); $process.Dispose() }
    $results += [PSCustomObject]@{file=$file;frames=$frames;fps=[int]$anim.Groups[3].Value;logicalWidth=$width;logicalHeight=$height;density=4;collisionMask='matches Git original'}
}
$expectedNames = @(rtk proxy git -C $repo ls-tree --name-only "${baseline}:Assets/Images/Game/Characters/Ichigo" | Where-Object { $_ -like '*.png' } | Sort-Object)
$actualNames = @(Get-ChildItem $spriteDir -Filter '*.png' | Select-Object -ExpandProperty Name | Sort-Object)
if(Compare-Object $expectedNames $actualNames) { throw 'Sprite PNG filename/count changed' }
$results | ConvertTo-Json -Depth 4 | Set-Content (Join-Path $PSScriptRoot 'validation.json') -Encoding UTF8
$totalFrames = ($results | Measure-Object frames -Sum).Sum
Write-Output "Verified $($results.Count)/$($animations.Count) character animation sheets, $totalFrames frames; $($actualNames.Count) PNG filenames unchanged; 4x dimensions and original collision masks match."
