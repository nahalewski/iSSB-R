$ErrorActionPreference='Stop'
$inputs=Get-Content (Join-Path $PSScriptRoot '../../../stage-inputs.json') -Raw | ConvertFrom-Json
$repo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
New-Item -ItemType Directory -Force (Join-Path $PSScriptRoot 'generated') | Out-Null
foreach($item in $inputs.backgrounds) {
 $dest=Join-Path $PSScriptRoot ('generated/'+$item.name+'-background.png')
 Copy-Item -LiteralPath $item.path -Destination $dest
 $item.path='generated/'+$item.name+'-background.png'
 Copy-Item -LiteralPath $dest -Destination (Join-Path $repo ('Assets/Images/Game/Stages/'+$item.name+'/bg_hd.png'))
}
foreach($item in $inputs.structures) {
 $dest='generated/'+$item.name.Replace('/','-')
 Copy-Item -LiteralPath $item.path -Destination (Join-Path $PSScriptRoot $dest)
 $item.path=$dest
}
$inputs | ConvertTo-Json -Depth 5 | Set-Content (Join-Path $PSScriptRoot 'generation.json')
