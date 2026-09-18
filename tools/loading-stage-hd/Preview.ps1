$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$repo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$root=Join-Path $repo 'Assets/Images/Menus'
$b=New-Object Drawing.Bitmap 1800,1200
$g=[Drawing.Graphics]::FromImage($b)
$g.ScaleTransform(2,2)
$g.InterpolationMode=[Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
$g.TextRenderingHint=[Drawing.Text.TextRenderingHint]::AntiAliasGridFit
$g.Clear([Drawing.Color]::FromArgb(243,244,246))
function Art($file,$x,$y,$w,$h) {
 $im=[Drawing.Bitmap]::FromFile((Join-Path $root $file))
 $g.DrawImage($im,[single]$x,[single]$y,[single]$w,[single]$h);$im.Dispose()
}
Art 'Buttons/Generic/back.png' 3 5 120 60
$g.FillRectangle([Drawing.Brushes]::Black,145,14,715,62)
$font=New-Object Drawing.Font 'Arial',28,([Drawing.FontStyle]::Bold),([Drawing.GraphicsUnit]::Pixel)
$g.DrawString('CHOOSE YOUR STAGE',$font,[Drawing.Brushes]::White,165,24)
$font.Dispose()
$names=@('test','hillside_battleground','battlefield','final_destination','hyruletemple','corneria')
for($i=0;$i -lt 6;$i++){Art ('StageSelect/'+$names[$i]+'.png') (40+280*($i%3)) (125+210*[Math]::Floor($i/3)) 260 180}
$font=New-Object Drawing.Font 'Arial',17,([Drawing.FontStyle]::Regular),([Drawing.GraphicsUnit]::Pixel)
$g.DrawString('Choose an arena to begin the battle.',$font,[Drawing.Brushes]::Black,40,545)
$font.Dispose();$g.Dispose()
$b.Save((Join-Path $PSScriptRoot 'preview.png'),[Drawing.Imaging.ImageFormat]::Png);$b.Dispose()
