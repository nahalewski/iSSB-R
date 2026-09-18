$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$repo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
Add-Type -ReferencedAssemblies System.Drawing -TypeDefinition @'
using System;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
public static class PauseArt {
 public static void Build(string path,string label,string subtitle,Color color,int icon) {
  using(var b=new Bitmap(1080,216))using(var g=Graphics.FromImage(b)) {
   g.ScaleTransform(3,3);g.SmoothingMode=SmoothingMode.AntiAlias;g.TextRenderingHint=TextRenderingHint.AntiAliasGridFit;
   g.Clear(Color.FromArgb(20,22,29));
   using(var brush=new SolidBrush(color))g.FillRectangle(brush,0,0,7,72);
   using(var brush=new SolidBrush(Color.FromArgb(36,39,48)))g.FillPolygon(brush,new Point[]{new Point(300,0),new Point(360,0),new Point(360,72),new Point(260,72)});
   using(var font=new Font("Arial",23,FontStyle.Bold|FontStyle.Italic,GraphicsUnit.Pixel))g.DrawString(label,font,Brushes.White,24,9);
   using(var font=new Font("Arial",11,FontStyle.Regular,GraphicsUnit.Pixel))g.DrawString(subtitle,font,Brushes.LightGray,26,43);
   using(var brush=new SolidBrush(color))using(var pen=new Pen(color,3)) {
    if(icon==0)g.FillPolygon(brush,new Point[]{new Point(321,24),new Point(321,48),new Point(340,36)});
    else if(icon==1){g.DrawRectangle(pen,320,25,21,16);g.DrawLine(pen,320,25,320,51);}
    else {g.DrawLine(pen,321,26,339,46);g.DrawLine(pen,339,26,321,46);}
   }
   b.Save(path,ImageFormat.Png);
   using(var f=File.Create(path+".hd")) {
    var header=System.Text.Encoding.ASCII.GetBytes("ISSBHD1 3 360 72\n");f.Write(header,0,header.Length);
    for(int i=0;i<360*72;i++)f.WriteByte(1);
   }
  }
 }
}
'@
$dir=Join-Path $repo 'Assets/Images/Game/HUD'
[PauseArt]::Build((Join-Path $dir 'ResumeButton.png'),'RESUME','RETURN TO THE BATTLE',[Drawing.Color]::FromArgb(70,215,173),0)
[PauseArt]::Build((Join-Path $dir 'FinishButton.png'),'END MATCH','RETURN TO CHARACTER SELECT',[Drawing.Color]::FromArgb(255,197,46),1)
[PauseArt]::Build((Join-Path $dir 'ExitButton.png'),'EXIT GAME','CLOSE ISSB-R',[Drawing.Color]::FromArgb(239,64,79),2)
Write-Output 'Built three density-3 pause buttons (1080x216 each).'
