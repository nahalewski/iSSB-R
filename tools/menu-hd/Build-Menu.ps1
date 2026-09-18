$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$assets = Join-Path $repo 'Assets/Images/Menus'
$jobs = Get-Content (Join-Path $PSScriptRoot 'generation-log.json') -Raw | ConvertFrom-Json
Add-Type -ReferencedAssemblies System.Drawing -TypeDefinition @'
using System;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
public static class MenuArt {
 public static void Save(Bitmap b,string path,int w,int h,int density) {
  b.Save(path,ImageFormat.Png);
  using(var f=File.Create(path+".hd")) {
   var header=System.Text.Encoding.ASCII.GetBytes("ISSBHD1 "+density+" "+w+" "+h+"\n");
   f.Write(header,0,header.Length);
   for(int i=0;i<w*h;i++)f.WriteByte(1);
  }
 }
 public static void Tile(string source,string path,int w,int h,string title,string subtitle,int cell,bool selected) {
  const int d=3;
  using(var src=new Bitmap(source)) using(var b=new Bitmap(w*d,h*d)) using(var g=Graphics.FromImage(b)) {
   g.InterpolationMode=InterpolationMode.HighQualityBicubic;
   g.SmoothingMode=SmoothingMode.AntiAlias;
   g.TextRenderingHint=TextRenderingHint.AntiAliasGridFit;
   Rectangle crop=new Rectangle(0,0,src.Width,src.Height);
   if(cell>=0) crop=new Rectangle((cell%2)*src.Width/2,(cell/2)*src.Height/4,src.Width/2,src.Height/4);
   g.DrawImage(src,new Rectangle(0,0,b.Width,b.Height),crop,GraphicsUnit.Pixel);
   if(w<100)g.Clear(Color.White);
   float size=title=="SMASH"?58:(title=="LOCAL WIRELESS"?23:(w<100?13:32));
   float y=title=="SMASH"?h-112:(h-size)/2-8;
   if(w<100)y=6;
   using(var font=new Font("Arial",size*d,FontStyle.Bold|FontStyle.Italic,GraphicsUnit.Pixel)) {
    float labelWidth=g.MeasureString(title,font,Int32.MaxValue,StringFormat.GenericTypographic).Width+30*d;
    using(var plate=new SolidBrush(Color.FromArgb(245,255,255,255)))
     g.FillRectangle(plate,0,(y-5)*d,Math.Min((w-6)*d,labelWidth),(size+12)*d);
    g.DrawString(title,font,Brushes.Black,12*d,y*d,StringFormat.GenericTypographic);
   }
   if(subtitle.Length>0) {
    using(var plate=new SolidBrush(Color.FromArgb(225,0,0,0)))g.FillRectangle(plate,12*d,(y+size+13)*d,(w-24)*d,23*d);
    using(var font=new Font("Arial",12*d,FontStyle.Bold,GraphicsUnit.Pixel))g.DrawString(subtitle,font,Brushes.White,20*d,(y+size+17)*d,StringFormat.GenericTypographic);
   }
   using(var pen=new Pen(selected?Color.FromArgb(255,224,0):Color.Black,(selected?6:2)*d))g.DrawRectangle(pen,3*d,3*d,b.Width-6*d,b.Height-6*d);
   if(selected)using(var pen=new Pen(Color.White,2*d))g.DrawRectangle(pen,8*d,8*d,b.Width-16*d,b.Height-16*d);
   Save(b,path,w,h,d);
  }
 }
 public static void Background(string source,string path) {
  using(var src=new Bitmap(source))using(var b=new Bitmap(1920,1280))using(var g=Graphics.FromImage(b)) {
   g.InterpolationMode=InterpolationMode.HighQualityBicubic;
   g.TextRenderingHint=TextRenderingHint.AntiAliasGridFit;
   g.DrawImage(src,0,0,b.Width,b.Height);
   g.FillRectangle(Brushes.White,0,1160,1920,120);
   using(var pen=new Pen(Color.LightGray,3))g.DrawLine(pen,0,1160,1920,1160);
   using(var font=new Font("Arial",58,FontStyle.Bold|FontStyle.Italic,GraphicsUnit.Pixel))g.DrawString("ISSB-R",font,Brushes.Black,1640,24);
   Save(b,path,480,320,4);
  }
 }
}
'@
function Source($name) { Join-Path $PSScriptRoot (($jobs | Where-Object name -eq $name).source) }
$tiles = @(
 @('smash','Buttons/Group/group',420,410,'SMASH','GROUP BATTLES',-1),
 @('solo','Buttons/Solo/solo',370,190,'SOLO','TRAINING & PRACTICE',-1),
 @('options','Buttons/Options/options',370,110,'OPTIONS','MAKE IT YOURS',-1),
 @('bluetooth','Buttons/Bluetooth/Bluetooth',370,80,'LOCAL WIRELESS','',-1),
 @('submenus','Buttons/Group/brawl',390,220,'BRAWL','START A BATTLE',0),
 @('submenus','Buttons/Group/rules',350,220,'RULES','SET THE MATCH',1),
 @('submenus','Buttons/Solo/training',600,300,'TRAINING','PRACTICE YOUR MOVES',2),
 @('submenus','Buttons/Options/controls',380,175,'CONTROLS','',3),
 @('submenus','Buttons/Options/sound',380,175,'SOUND','',4),
 @('submenus','Buttons/Options/display',380,175,'DISPLAY','',5),
 @('submenus','Buttons/Options/other',380,175,'OTHER','',6),
 @('submenus','Buttons/Generic/back',64,32,'BACK','',7)
)
foreach($t in $tiles) {
 foreach($selected in @($false,$true)) {
  $suffix=if($selected){'_selected.png'}else{'.png'}
  [MenuArt]::Tile((Source $t[0]),(Join-Path $assets ($t[1]+$suffix)),$t[2],$t[3],$t[4],$t[5],$t[6],$selected)
 }
}
[MenuArt]::Background((Source 'background'),(Join-Path $assets 'Backgrounds/bg2.png'))
Write-Output 'Installed 24 HD button states and the HD menu background.'
$titleSource=Join-Path $PSScriptRoot 'sources/title-issb-r.png'
if(Test-Path $titleSource) {
 $src=[Drawing.Bitmap]::FromFile($titleSource)
 $title=New-Object Drawing.Bitmap 1440,960
 $g=[Drawing.Graphics]::FromImage($title)
 $g.InterpolationMode=[Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
 $g.DrawImage($src,0,0,1440,960)
 $g.Dispose();$src.Dispose()
 [MenuArt]::Save($title,(Join-Path $assets 'titlescreen.png'),480,320,3)
 $title.Dispose()
}
