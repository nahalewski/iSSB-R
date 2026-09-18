param(
    [Parameter(Mandatory=$true)][string]$Name,
    [Parameter(Mandatory=$true)][string]$Generated,
    [Parameter(Mandatory=$true)][int]$Frames
)
$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing
Add-Type -TypeDefinition @'
using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Diagnostics;
public static class SpritePacker {
    static Rectangle Bounds(Bitmap b, int top, int bottom) {
        int left=b.Width, right=-1, first=bottom, last=-1;
        var background=b.GetPixel(0,0);
        for(int y=top;y<bottom;y++) for(int x=0;x<b.Width;x++) {
            var pixel=b.GetPixel(x,y);
            if(pixel.A>16 && !(pixel.G>240 && pixel.R<20 && pixel.B<20) && !(background.A==255 && pixel.ToArgb()==background.ToArgb())) { left=Math.Min(left,x); right=Math.Max(right,x); first=Math.Min(first,y); last=Math.Max(last,y); }
        }
        if(right<left) throw new Exception("Empty frame");
        return Rectangle.FromLTRB(left,first,right+1,last+1);
    }
    static Bitmap Original(string path) {
        // Read the fixed pre-remaster revision, including matte colors and alpha.
        // This makes repeated packing independent of the currently installed PNG.
        var repo=Path.GetFullPath(Path.Combine(Path.GetDirectoryName(path),"../../../../../")).TrimEnd('\\','/');
        var start=new ProcessStartInfo("rtk", "proxy git -C \""+repo+"\" show 837ad567fcf1a969665fde3fd4057d7f18999e3d:Assets/Images/Game/Characters/Ichigo/"+Path.GetFileName(path));
        start.UseShellExecute=false; start.RedirectStandardOutput=true; start.CreateNoWindow=true;
        using(var process=Process.Start(start)) using(var memory=new MemoryStream()) {
            process.StandardOutput.BaseStream.CopyTo(memory); process.WaitForExit();
            if(process.ExitCode!=0) throw new Exception("Cannot read original sprite from Git");
            memory.Position=0;
            using(var original=new Bitmap(memory)) return new Bitmap(original);
        }
    }
    static int[] SplitRows(Bitmap b,int frames) {
        var gaps=new List<Tuple<int,int>>();
        bool seen=false; int start=-1;
        for(int y=0;y<b.Height;y++) {
            bool occupied=false;
            for(int x=0;x<b.Width;x++) if(b.GetPixel(x,y).A>16) { occupied=true; break; }
            if(!occupied && seen && start<0) start=y;
            if(occupied) {
                if(start>=0) { gaps.Add(Tuple.Create(start,y)); start=-1; }
                seen=true;
            }
        }
        var selected=gaps.Where(g=>g.Item2-g.Item1>=2).OrderByDescending(g=>g.Item2-g.Item1).Take(frames-1).ToArray();
        if(selected.Length!=frames-1) throw new Exception("Cannot separate expected "+frames+" frames; image needs regeneration");
        return new[]{0}.Concat(selected.Select(g=>(g.Item1+g.Item2)/2).OrderBy(y=>y)).Concat(new[]{b.Height}).ToArray();
    }
    public static void Pack(string original, string generated, int frames) {
        using(var src=Original(original)) using(var hd=new Bitmap(generated)) {
            if(hd.GetPixel(0,0).A!=0) throw new Exception("Generated image has no transparent background");
            const int density=4;
            int cellH=src.Height/frames;
            var cuts=SplitRows(hd,frames);
            using(var output=new Bitmap(src.Width*density,src.Height*density,PixelFormat.Format32bppArgb)) {
                using(var g=Graphics.FromImage(output)) {
                    g.Clear(Color.Transparent);
                    g.CompositingMode=CompositingMode.SourceCopy;
                    g.InterpolationMode=InterpolationMode.HighQualityBicubic;
                    g.PixelOffsetMode=PixelOffsetMode.HighQuality;
                    for(int f=0;f<frames;f++) {
                        var target=Bounds(src,f*cellH,(f+1)*cellH);
                        var source=Bounds(hd,cuts[f],cuts[f+1]);
                        var dest=new Rectangle(target.X*density,target.Y*density,target.Width*density,target.Height*density);
                        g.DrawImage(hd,dest,source,GraphicsUnit.Pixel);
                    }
                }
                output.Save(original+".new",ImageFormat.Png);
            }
            using(var stream=File.Create(original+".hd")) {
                var header=System.Text.Encoding.ASCII.GetBytes("ISSBHD1 "+density+" "+src.Width+" "+src.Height+"\n");
                stream.Write(header,0,header.Length);
                for(int y=0;y<src.Height;y++) for(int x=0;x<src.Width;x++) stream.WriteByte((byte)(src.GetPixel(x,y).A>0?1:0));
            }
        }
        File.Copy(original+".new",original,true);
        File.Delete(original+".new");
    }
}
'@ -ReferencedAssemblies System.Drawing,System.Core
$repoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$sprite = Join-Path $repoRoot "Assets/Images/Game/Characters/Ichigo/$Name.png"
[SpritePacker]::Pack($sprite, $Generated, $Frames)
Write-Output "Packed $Name : $Frames frames, 4x density, original collision mask preserved."
