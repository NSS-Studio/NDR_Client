

$version="0.75"
$env:Path="C:\Qt\5.9.3\mingw53_32\bin\;C:/Qt/Tools/mingw530_32/bin/;"+$env.PATH
$BUILD_TEST=Test-Path .\build
if ($BUILD_TEST) {
    Remove-Item -Recurse -Force .\build
}
Set-Location client\ts-file
.".\update.ps1"
.".\release.ps1"
Set-Location ..\..

New-Item build -type directory
Set-Location build

qmake.exe ..\ndr-client.pro -spec win32-g++ CONFIG+=debug
mingw32-make.exe all -j 4
Set-Location ..