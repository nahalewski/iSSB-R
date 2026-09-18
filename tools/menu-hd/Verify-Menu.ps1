$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$repo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$assets=Join-Path $repo 'Assets/Images/Menus'
$code=Get-Content (Join-Path $repo 'Source/SmashBros/Menus/MenuClasses.cpp') -Raw
if((Get-Content (Join-Path $repo 'Source/Game.cpp') -Raw) -match 'showMessage[^;]*versionMessage') { throw 'Startup donation popup remains' }
$screens=@('MainMenu','GroupMenu','SoloMenu','OptionsMenu')
foreach($screen in $screens) {
 $body=[regex]::Match($code,'void '+$screen+'::Initialize\(\)\s*\{([\s\S]*?)\n\tvoid ').Groups[1].Value
 $matches=[regex]::Matches($body,'addItem\((\d+), (\d+), "Images/Menus/([^\"]+)", "Images/Menus/([^\"]+)", "([^\"]+)"\)')
 $rects=@()
 $b=New-Object Drawing.Bitmap 1800,1200
 $g=[Drawing.Graphics]::FromImage($b)
 $g.InterpolationMode=[Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
 $bg=[Drawing.Bitmap]::FromFile((Join-Path $assets 'Backgrounds/bg2.png'))
 $g.DrawImage($bg,0,0,1800,1200); $bg.Dispose()
 foreach($m in $matches) {
  $x=[int]$m.Groups[1].Value; $y=[int]$m.Groups[2].Value
  $normal=Join-Path $assets $m.Groups[3].Value
  $hover=Join-Path $assets $m.Groups[4].Value
  $dims=@()
  foreach($path in @($normal,$hover)) {
   $bytes=[IO.File]::ReadAllBytes($path+'.hd'); $end=[Array]::IndexOf($bytes,[byte]10)
   $header=[Text.Encoding]::ASCII.GetString($bytes,0,$end)
   if($header -notmatch '^ISSBHD1 (\d+) (\d+) (\d+)$'){throw "Invalid metadata $path"}
   $d=[int]$Matches[1];$w=[int]$Matches[2];$h=[int]$Matches[3]
   $im=[Drawing.Bitmap]::FromFile($path)
   if($im.Width -ne $w*$d -or $im.Height -ne $h*$d -or $bytes.Length -ne $end+1+$w*$h){throw "Invalid dimensions $path"}
   $im.Dispose();$dims+=,"$w,$h"
  }
  if($dims[0] -ne $dims[1]){throw 'Hover changes hit area'}
  $r=New-Object Drawing.RectangleF ($x-$w/2),($y-$h/2),$w,$h
  if($r.Left -lt 0 -or $r.Right -gt 900 -or $r.Top -lt 70 -or $r.Bottom -gt 545){throw "Tile outside menu area: $normal"}
  foreach($other in $rects){if($r.IntersectsWith($other)){throw 'Overlapping menu hit areas'}}
  $rects+=,$r
  $im=[Drawing.Bitmap]::FromFile($normal)
  if($m.Groups[5].Value -ne 'BluetoothMenu') {
   $g.DrawImage($im,[single]($r.X*2),[single]($r.Y*2),[single]($r.Width*2),[single]($r.Height*2))
  }
  $im.Dispose()
 }
 $back=[Drawing.Bitmap]::FromFile((Join-Path $assets 'Buttons/Generic/back.png'))
 $g.DrawImage($back,6,10,240,120);$back.Dispose()
 $g.Dispose();$b.Save((Join-Path $PSScriptRoot ($screen+'-preview.png')),[Drawing.Imaging.ImageFormat]::Png);$b.Dispose()
 Write-Output "$screen : $($rects.Count) destinations; paired HD states; no overlapping or offscreen hit areas."
}
