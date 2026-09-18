$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$repo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$root=Join-Path $repo 'Assets/Images/Menus'
$b=New-Object Drawing.Bitmap 1800,1200
$g=[Drawing.Graphics]::FromImage($b)
$g.ScaleTransform(2,2)
$g.InterpolationMode=[Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
$g.TextRenderingHint=[Drawing.Text.TextRenderingHint]::AntiAliasGridFit
$g.Clear([Drawing.Color]::FromArgb(238,239,242))
function Art($file,$x,$y,$w,$h) {
 $img=[Drawing.Bitmap]::FromFile((Join-Path $root $file))
 $g.DrawImage($img,[single]$x,[single]$y,[single]$w,[single]$h);$img.Dispose()
}
function Label($text,$x,$y,$size,$color) {
 $font=New-Object Drawing.Font 'Arial',$size,([Drawing.FontStyle]::Bold),([Drawing.GraphicsUnit]::Pixel)
 $brush=New-Object Drawing.SolidBrush $color
 $g.DrawString($text,$font,$brush,$x,$y);$font.Dispose();$brush.Dispose()
}
$g.FillRectangle([Drawing.Brushes]::White,0,0,900,76)
$brush=New-Object Drawing.SolidBrush ([Drawing.Color]::FromArgb(238,239,242));$g.FillRectangle($brush,0,76,900,524);$brush.Dispose()
$g.FillRectangle([Drawing.Brushes]::Black,0,76,900,45)
Label 'CHOOSE YOUR FIGHTER' 28 81 26 ([Drawing.Color]::White)
Art 'Buttons/Generic/back.png' 3 5 120 60
Art 'Buttons/Group/freeforall.png' 140 14 240 42
Label '02     MINUTE TIMED BATTLE' 506 30 20 ([Drawing.Color]::Black)
$names=@('mario','ichigo','sonic','fox','pikachu','link')
for($i=0;$i -lt 6;$i++){Art ('CharacterSelect/icons/'+$names[$i]+'.png') (27+142*$i) 129 136 150}
Label 'Drag a player token onto a fighter. Tap a player panel to change its type.' 28 286 15 ([Drawing.Color]::FromArgb(55,57,65))
Art 'Buttons/Generic/readytofight.png' 30 315 840 44
$cards=@('p1_red','p2_blue','na','na')
for($i=0;$i -lt 4;$i++) {
 $cx=112+225*$i
 Art ('CharacterSelect/cards/'+$cards[$i]+'.png') ($cx-100) 375 200 190
 if($i -lt 2) {
  $name=@('ichigo','sonic')[$i]
  $im=[Drawing.Bitmap]::FromFile((Join-Path $root ('CharacterSelect/previews/'+$name+'.png')))
  $w=$im.Width/3;$h=$im.Height/3;$im.Dispose()
  Art ('CharacterSelect/previews/'+$name+'.png') ($cx-$w/2) (470-$h/2) $w $h
  Label $name.ToUpper() ($cx-33) 536 18 ([Drawing.Color]::White)
 }
 Art ('CharacterSelect/coins/coin'+($i+1)+'.png') ($cx-94) 403 42 42
}
$g.Dispose();$b.Save((Join-Path $PSScriptRoot 'preview.png'),[Drawing.Imaging.ImageFormat]::Png);$b.Dispose()
