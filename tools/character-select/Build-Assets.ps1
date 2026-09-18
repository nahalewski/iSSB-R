$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$repo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$root=Join-Path $repo 'Assets/Images/Menus'
Add-Type -ReferencedAssemblies System.Drawing -TypeDefinition @'
using System;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
public static class SelectArt {
 public static void Portrait(Graphics g,string path,RectangleF dest) {
  using(var src=new Bitmap(path)) {
   int left=src.Width,top=src.Height,right=0,bottom=0;
   for(int y=0;y<src.Height;y++)for(int x=0;x<src.Width;x++)if(src.GetPixel(x,y).A>16){left=Math.Min(left,x);top=Math.Min(top,y);right=Math.Max(right,x);bottom=Math.Max(bottom,y);}
   if(Path.GetFileName(path)=="ichigo.png")bottom=Math.Min(bottom,790);
   float sw=right-left+1,sh=bottom-top+1;
   float scale=Math.Min(dest.Width/sw,dest.Height/sh);
   g.InterpolationMode=InterpolationMode.HighQualityBicubic;
   g.DrawImage(src,new RectangleF(dest.X+(dest.Width-sw*scale)/2,dest.Y+(dest.Height-sh*scale)/2,sw*scale,sh*scale),new RectangleF(left,top,sw,sh),GraphicsUnit.Pixel);
  }
 }
 public static void Preview(string source,string path) {
  using(var b=new Bitmap(480,360))using(var g=Graphics.FromImage(b)) {
   g.Clear(Color.Transparent);g.ScaleTransform(3,3);
   Portrait(g,source,new RectangleF(0,0,160,120));Save(b,path,160,120);
  }
 }
 public static void Save(Bitmap b,string path,int w,int h) {
  b.Save(path,ImageFormat.Png);
  using(var f=File.Create(path+".hd")) {
   byte[] header=System.Text.Encoding.ASCII.GetBytes("ISSBHD1 3 "+w+" "+h+"\n");f.Write(header,0,header.Length);
   for(int y=0;y<h;y++)for(int x=0;x<w;x++)f.WriteByte(b.GetPixel(x*3+1,y*3+1).A>16?(byte)1:(byte)0);
  }
 }
 public static void Make(string path,int w,int h,string text,Color color,string portrait,bool selected) {
  using(var b=new Bitmap(w*3,h*3))using(var g=Graphics.FromImage(b)) {
   g.ScaleTransform(3,3);g.SmoothingMode=SmoothingMode.AntiAlias;g.TextRenderingHint=TextRenderingHint.AntiAliasGridFit;
   g.Clear(color);
   using(var brush=new SolidBrush(Color.FromArgb(25,255,255,255)))g.FillPolygon(brush,new Point[]{new Point(w/2,0),new Point(w,0),new Point(w/2,h),new Point(0,h)});
   if(!String.IsNullOrEmpty(portrait)) {
    g.FillRectangle(Brushes.White,3,3,w-6,h-33);
    Portrait(g,portrait,new RectangleF(5,5,w-10,h-38));
   }
   else if(h>100)g.FillRectangle(Brushes.Black,0,h-32,w,32);
   using(var font=new Font("Arial",!String.IsNullOrEmpty(portrait)?16:(h>100?17:22),FontStyle.Bold|FontStyle.Italic,GraphicsUnit.Pixel)) {
    float tx=!String.IsNullOrEmpty(portrait)?(w-g.MeasureString(text,font).Width)/2:16;
    g.DrawString(text,font,Brushes.White,tx,!String.IsNullOrEmpty(portrait)?h-27:10);
   }
   using(var pen=new Pen(selected?Color.Yellow:Color.FromArgb(22,22,26),selected?5:2))g.DrawRectangle(pen,2,2,w-4,h-4);
   Save(b,path,w,h);
  }
 }
 public static void Coin(string path,int number,Color color) {
  using(var b=new Bitmap(126,126))using(var g=Graphics.FromImage(b)) {
   g.SmoothingMode=SmoothingMode.AntiAlias;
   using(var brush=new SolidBrush(color))g.FillEllipse(brush,3,3,120,120);
   using(var pen=new Pen(Color.White,6))g.DrawEllipse(pen,6,6,114,114);
   using(var font=new Font("Arial",48,FontStyle.Bold,GraphicsUnit.Pixel))g.DrawString("P"+number,font,Brushes.Black,25,34);
   Save(b,path,42,42);
  }
 }
}
'@
$names=@('mario','ichigo','sonic','fox','pikachu','link')
foreach($name in $names) {
 $source=Join-Path $PSScriptRoot "portraits/$name.png"
 [SelectArt]::Make((Join-Path $root "CharacterSelect/icons/$name.png"),136,150,$name.ToUpper(),[Drawing.Color]::FromArgb(25,26,32),$source,$false)
 [SelectArt]::Preview($source,(Join-Path $root "CharacterSelect/previews/$name.png"))
}
$colors=@([Drawing.Color]::FromArgb(222,35,48),[Drawing.Color]::FromArgb(26,104,230),[Drawing.Color]::FromArgb(209,150,0),[Drawing.Color]::FromArgb(16,158,87))
$files=@('p1_red','p2_blue','p3_yellow','p4_green')
for($i=0;$i -lt 4;$i++) {
 [SelectArt]::Make((Join-Path $root ('CharacterSelect/cards/'+$files[$i]+'.png')),200,190,('P'+($i+1)+'  /  HUMAN'),$colors[$i],$null,$false)
 [SelectArt]::Coin((Join-Path $root ('CharacterSelect/coins/coin'+($i+1)+'.png')),($i+1),$colors[$i])
}
[SelectArt]::Coin((Join-Path $root 'CharacterSelect/coins/coin.png'),1,$colors[0])
[SelectArt]::Make((Join-Path $root 'CharacterSelect/cards/cpu.png'),200,190,'CPU',[Drawing.Color]::FromArgb(70,73,85),$null,$false)
[SelectArt]::Make((Join-Path $root 'CharacterSelect/cards/na.png'),200,190,'TAP TO JOIN',[Drawing.Color]::FromArgb(44,46,54),$null,$false)
foreach($selected in @($false,$true)) {
 $suffix=if($selected){'_selected.png'}else{'.png'}
 [SelectArt]::Make((Join-Path $root ('Buttons/Generic/readytofight'+$suffix)),840,44,'READY TO FIGHT!',[Drawing.Color]::FromArgb(230,35,45),$null,$selected)
 [SelectArt]::Make((Join-Path $root ('Buttons/Group/freeforall'+$suffix)),240,42,'FREE FOR ALL',[Drawing.Color]::FromArgb(25,26,32),$null,$selected)
 [SelectArt]::Make((Join-Path $root ('Buttons/Group/teambattle'+$suffix)),240,42,'TEAM BATTLE',[Drawing.Color]::FromArgb(25,26,32),$null,$selected)
}
[SelectArt]::Make((Join-Path $root 'Buttons/Solo/training_mode.png'),240,42,'TRAINING',[Drawing.Color]::FromArgb(25,26,32),$null,$false)
Write-Output 'Built modern roster, player panels, tokens and match banners.'
