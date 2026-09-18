$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$repo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$assets=Join-Path $repo 'Assets/Images'
Add-Type -ReferencedAssemblies System.Drawing -TypeDefinition @'
using System;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
public static class MenuArt {
 public static void Save(Bitmap b,string path,int w,int h) {
  b.Save(path,ImageFormat.Png);
  using(var f=File.Create(path+".hd")) {
   var hdr=System.Text.Encoding.ASCII.GetBytes("ISSBHD1 3 "+w+" "+h+"\n");f.Write(hdr,0,hdr.Length);
   for(int i=0;i<w*h;i++)f.WriteByte(1);
  }
 }
 public static void Text(Graphics g,string s,float x,float y,float size,Brush brush,bool center) {
  using(var font=new Font("Arial",size,FontStyle.Bold|FontStyle.Italic,GraphicsUnit.Pixel)) {
   if(center)x-=g.MeasureString(s,font).Width/2;g.DrawString(s,font,brush,x,y);
  }
 }
 public static void Loading(string path) {
  using(var b=new Bitmap(2700,1800))using(var g=Graphics.FromImage(b)) {
   g.ScaleTransform(3,3);g.SmoothingMode=SmoothingMode.AntiAlias;g.TextRenderingHint=TextRenderingHint.AntiAliasGridFit;g.Clear(Color.FromArgb(243,244,246));
   using(var gray=new SolidBrush(Color.FromArgb(230,232,237)))g.FillPolygon(gray,new Point[]{new Point(380,0),new Point(850,0),new Point(520,600),new Point(50,600)});
   using(var red=new SolidBrush(Color.FromArgb(230,35,45))) {
    g.FillPolygon(red,new Point[]{new Point(0,0),new Point(390,0),new Point(0,150)});
    g.FillPolygon(red,new Point[]{new Point(900,600),new Point(510,600),new Point(900,450)});
   }
   Text(g,"ISSB-R",450,190,120,Brushes.Black,true);
   Text(g,"READY TO BRAWL",450,325,26,Brushes.Black,true);
   Text(g,"LOADING",450,424,18,Brushes.Black,true);
   using(var gray=new SolidBrush(Color.FromArgb(209,212,218)))g.FillRectangle(gray,210,474,480,6);
   Save(b,path,900,600);
  }
 }
 public static void Card(string bgPath,string stagePath,string label,string path,bool sheet) {
  using(var b=new Bitmap(780,540))using(var g=Graphics.FromImage(b))using(var bg=new Bitmap(bgPath)) {
   g.ScaleTransform(3,3);g.InterpolationMode=InterpolationMode.HighQualityBicubic;g.TextRenderingHint=TextRenderingHint.AntiAliasGridFit;
   g.Clear(Color.FromArgb(22,24,30));
   float scale=Math.Max(260f/bg.Width,146f/bg.Height);
   g.SetClip(new Rectangle(0,0,260,146));g.DrawImage(bg,(260-bg.Width*scale)/2,(146-bg.Height*scale)/2,bg.Width*scale,bg.Height*scale);
   if(!String.IsNullOrEmpty(stagePath))using(var stage=new Bitmap(stagePath)) {
    int sh=sheet?stage.Height/6:stage.Height,l=stage.Width,t=sh,r=0,bot=0;
    for(int y=0;y<sh;y++)for(int x=0;x<stage.Width;x++)if(stage.GetPixel(x,y).A>16){l=Math.Min(l,x);t=Math.Min(t,y);r=Math.Max(r,x);bot=Math.Max(bot,y);}
    float sw=r-l+1,hh=bot-t+1,s=Math.Min(232/sw,122/hh);
    g.DrawImage(stage,new RectangleF((260-sw*s)/2,13+(122-hh*s)/2,sw*s,hh*s),new RectangleF(l,t,sw,hh),GraphicsUnit.Pixel);
   }
   g.ResetClip();g.FillRectangle(Brushes.Black,0,146,260,34);
   Text(g,label,12,153,label.Length>19?16:18,Brushes.White,false);
   using(var p=new Pen(Color.FromArgb(28,30,37),2))g.DrawRectangle(p,1,1,258,178);
   Save(b,path,260,180);
  }
 }
}
'@
[MenuArt]::Loading((Join-Path $assets 'loading.png'))
$rows=@(
 @('test','TEST ARENA','TestStage',''),
 @('hillside_battleground','HILLSIDE BATTLEGROUND','HillsideBattleground','mainstage.png'),
 @('battlefield','BATTLEFIELD','BattlefieldBrawl','mainstage.png'),
 @('final_destination','FINAL DESTINATION','FinalDestinationBrawl','mainstage.png'),
 @('hyruletemple','HYRULE TEMPLE','HyruleTemple','mainstage.png'),
 @('corneria','CORNERIA','Corneria','plat.png')
)
foreach($row in $rows) {
 $dir=Join-Path $assets ('Game/Stages/'+$row[2])
 $stage=if($row[3]){Join-Path $dir $row[3]}else{''}
 [MenuArt]::Card((Join-Path $dir 'bg_hd.png'),$stage,$row[1],(Join-Path $assets ('Menus/StageSelect/'+$row[0]+'.png')),($row[0] -eq 'final_destination'))
}
Write-Output 'Built 2700x1800 loading artwork and six 780x540 level cards.'
