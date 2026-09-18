$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$repo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
Add-Type -ReferencedAssemblies System.Drawing -TypeDefinition @'
using System;
using System.IO;
using System.Drawing;
public static class BattleCheck {
 public static void Check(string path,string original) {
  var bytes=File.ReadAllBytes(path+".hd");int end=Array.IndexOf(bytes,(byte)10);
  var fields=System.Text.Encoding.ASCII.GetString(bytes,0,end).Split(' ');
  if(fields[0]!="ISSBHD1")throw new Exception("Bad metadata: "+path);
  int density=int.Parse(fields[1]),w=int.Parse(fields[2]),h=int.Parse(fields[3]);
  using(var b=new Bitmap(path)) {
   if(b.Width!=w*density||b.Height!=h*density||bytes.Length!=end+1+w*h)throw new Exception("Invalid asset dimensions: "+path);
   if(!String.IsNullOrEmpty(original))using(var old=new Bitmap(original)) {
    if(old.Width!=w||old.Height!=h)throw new Exception("Logical size changed: "+path);
    for(int y=0;y<h;y++)for(int x=0;x<w;x++) {
     var expected=old.GetPixel(x,y).A>0?1:0;
     if(bytes[end+1+y*w+x]!=expected)throw new Exception("Collision mask changed: "+path);
     if((b.GetPixel(x*density,y*density).A>0?1:0)!=expected)throw new Exception("Silhouette changed: "+path);
    }
   }
  }
 }
}
'@
$inputs=Get-Content (Join-Path $PSScriptRoot 'generation.json') -Raw | ConvertFrom-Json
foreach($item in $inputs.structures) {
 [BattleCheck]::Check((Join-Path $repo ('Assets/Images/Game/Stages/'+$item.name)),(Join-Path $PSScriptRoot ('originals/'+$item.name)))
}
$controls=Get-ChildItem (Join-Path $repo 'Assets/Images/Game/Controls') -Filter '*.png'
foreach($file in $controls){[BattleCheck]::Check($file.FullName,$null)}
Write-Output ('PASS: all 5 stage sizes, silhouettes and collision masks unchanged; '+$controls.Count+' HD control textures valid.')
