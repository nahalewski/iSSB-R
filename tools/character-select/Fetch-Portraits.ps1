$ErrorActionPreference='Stop'
$dir=Join-Path $PSScriptRoot 'portraits'
New-Item -ItemType Directory -Force $dir | Out-Null
$sources=@{}
foreach($name in @('mario','sonic','fox','pikachu','link')) {
 $sources[$name]="https://www.smashbros.com/assets_v2/img/fighter/$name/main.png"
}
$sources['ichigo']='https://www.bleach-bravesouls.com/assets/img/character/appearance/01/character_ichigo_01.png'
foreach($name in $sources.Keys) {
 Invoke-WebRequest -UseBasicParsing $sources[$name] -OutFile (Join-Path $dir "$name.png")
}
$sources | ConvertTo-Json | Set-Content (Join-Path $dir 'sources.json')
