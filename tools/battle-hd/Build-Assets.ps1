$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$repo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$stages=Join-Path $repo 'Assets/Images/Game/Stages'
$controls=Join-Path $repo 'Assets/Images/Game/Controls'
Add-Type -ReferencedAssemblies System.Drawing -TypeDefinition @'
using System;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
public static class BattleArt {
 public static Rectangle Bounds(Bitmap b) {
  int l=b.Width,t=b.Height,r=-1,bot=-1;
  for(int y=0;y<b.Height;y++)for(int x=0;x<b.Width;x++)if(b.GetPixel(x,y).A>16){l=Math.Min(l,x);t=Math.Min(t,y);r=Math.Max(r,x);bot=Math.Max(bot,y);}
  return r<0?new Rectangle(0,0,b.Width,b.Height):new Rectangle(l,t,r-l+1,bot-t+1);
 }
 public static void Save(Bitmap b,string path,Bitmap logical,int density) {
  b.Save(path,ImageFormat.Png);
  using(var f=File.Create(path+".hd")) {
   var hdr=System.Text.Encoding.ASCII.GetBytes("ISSBHD1 "+density+" "+logical.Width+" "+logical.Height+"\n");f.Write(hdr,0,hdr.Length);
   for(int y=0;y<logical.Height;y++)for(int x=0;x<logical.Width;x++)f.WriteByte(logical.GetPixel(x,y).A>0?(byte)1:(byte)0);
  }
 }
 public static void Stage(string original,string art,string dest) {
  using(var old=new Bitmap(original))using(var src=new Bitmap(art))using(var b=new Bitmap(old.Width*2,old.Height*2)) {
   var target=Bounds(old);var source=Bounds(src);
   using(var g=Graphics.FromImage(b)){g.InterpolationMode=InterpolationMode.HighQualityBicubic;g.DrawImage(src,new Rectangle(target.X*2,target.Y*2,target.Width*2,target.Height*2),source,GraphicsUnit.Pixel);}
   // Keep the reference alpha silhouette and the exact logical collision mask.
   for(int y=0;y<b.Height;y++)for(int x=0;x<b.Width;x++) {
    var a=old.GetPixel(x/2,y/2).A;var c=b.GetPixel(x,y);b.SetPixel(x,y,Color.FromArgb(a,c.R,c.G,c.B));
   }
   Save(b,dest,old,2);
  }
 }
 public static void Button(string original,string dest,string name) {
  using(var old=new Bitmap(original))using(var b=new Bitmap(old.Width*4,old.Height*4))using(var g=Graphics.FromImage(b)) {
   float w=b.Width,h=b.Height,cx=w/2,cy=h/2,r=Math.Min(w,h)/2-6;
   g.SmoothingMode=SmoothingMode.AntiAlias;g.TextRenderingHint=TextRenderingHint.AntiAliasGridFit;g.Clear(Color.Transparent);
   using(var brush=new SolidBrush(Color.FromArgb(235,23,27,37)))g.FillEllipse(brush,cx-r,cy-r,r*2,r*2);
   using(var pen=new Pen(Color.FromArgb(240,245,250),5))g.DrawEllipse(pen,cx-r,cy-r,r*2,r*2);
   if(name.StartsWith("joystick")) {
    float dx=0,dy=0,d=name.EndsWith("2")?r*.46f:r*.3f;
    if(name.Contains("left"))dx=-d;if(name.Contains("right"))dx=d;if(name.Contains("up"))dy=-d;if(name.Contains("down"))dy=d;
    using(var brush=new SolidBrush(Color.FromArgb(235,225,235,250)))g.FillEllipse(brush,cx+dx-r*.37f,cy+dy-r*.37f,r*.74f,r*.74f);
   } else {
    string label=name=="button_a"?"A":name=="button_b"?"B":name=="button_xy"?"X":name=="button_z"?"Z":name=="button_start"?"II":"";
    if(name.StartsWith("arrow")) {
     g.TranslateTransform(cx,cy);float angle=name=="arrow_up"?0:name=="arrow_right"?90:name=="arrow_down"?180:270;g.RotateTransform(angle);
     g.FillPolygon(Brushes.White,new PointF[]{new PointF(0,-r*.48f),new PointF(r*.45f,r*.25f),new PointF(-r*.45f,r*.25f)});g.ResetTransform();
    } else using(var font=new Font("Arial",r*.95f,FontStyle.Bold,GraphicsUnit.Pixel)) {
     var size=g.MeasureString(label,font);g.DrawString(label,font,Brushes.White,cx-size.Width/2,cy-size.Height/2);
    }
   }
   using(var mask=new Bitmap(old.Width,old.Height)) {
    for(int y=0;y<mask.Height;y++)for(int x=0;x<mask.Width;x++)mask.SetPixel(x,y,b.GetPixel(x*4+2,y*4+2));
    Save(b,dest,mask,4);
   }
  }
 }
}
'@
$inputs=Get-Content (Join-Path $PSScriptRoot 'generation.json') -Raw | ConvertFrom-Json
foreach($item in $inputs.structures) {
 $dest=Join-Path $stages $item.name
 $original=Join-Path $PSScriptRoot ('originals/'+$item.name)
 New-Item -ItemType Directory -Force (Split-Path $original) | Out-Null
 if(!(Test-Path $original)){Copy-Item -LiteralPath $dest -Destination $original}
 [BattleArt]::Stage($original,(Join-Path $PSScriptRoot $item.path),$dest)
}
foreach($file in Get-ChildItem $controls -Filter '*.png') {
 $original=Join-Path $PSScriptRoot ('originals/controls/'+$file.Name)
 New-Item -ItemType Directory -Force (Split-Path $original) | Out-Null
 if(!(Test-Path $original)){Copy-Item -LiteralPath $file.FullName -Destination $original}
 [BattleArt]::Button($original,$file.FullName,$file.BaseName)
}
Write-Output 'Built 5 HD stage structures and all 26 HD touch-control textures.'
