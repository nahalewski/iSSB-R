$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$res=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../Projects/AndroidStudio/app/src/main/res'))
$src=[Drawing.Bitmap]::FromFile((Join-Path $PSScriptRoot 'issbr-icon.png'))
foreach($entry in @(@('mdpi',48),@('hdpi',72),@('xhdpi',96),@('xxhdpi',144),@('xxxhdpi',192))) {
 $dir=Join-Path $res ('mipmap-'+$entry[0])
 $size=[int]$entry[1]
 foreach($round in @($false,$true)) {
  $b=New-Object Drawing.Bitmap $size,$size
  $g=[Drawing.Graphics]::FromImage($b)
  $g.InterpolationMode=[Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
  $g.SmoothingMode=[Drawing.Drawing2D.SmoothingMode]::AntiAlias
  if($round) {
   $clip=New-Object Drawing.Drawing2D.GraphicsPath
   $clip.AddEllipse(0,0,$size,$size);$g.SetClip($clip);$clip.Dispose()
  }
  $g.DrawImage($src,0,0,$size,$size);$g.Dispose()
  $name=if($round){'issbr_launcher_round.png'}else{'issbr_launcher.png'}
  $b.Save((Join-Path $dir $name),[Drawing.Imaging.ImageFormat]::Png);$b.Dispose()
 }
}
$dir=Join-Path $res 'drawable-nodpi'
New-Item -ItemType Directory -Force $dir | Out-Null
$b=New-Object Drawing.Bitmap 1024,1024
$g=[Drawing.Graphics]::FromImage($b)
$g.InterpolationMode=[Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
$g.DrawImage($src,0,0,1024,1024);$g.Dispose();$src.Dispose()
$b.Save((Join-Path $dir 'issbr_icon_art.png'),[Drawing.Imaging.ImageFormat]::Png);$b.Dispose()
Write-Output 'Built 10 density variants and adaptive icon artwork.'
