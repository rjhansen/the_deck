#!/usr/bin/env pwsh

$version = "1.0.0"
$win_output = the_deck-win64-$version

If ($IsWindows) {
    Remove-Item -Recurse -Force ..\the_deck-win64-$version
    New-Item -Name $win_output -ItemType Directory
    New-Item -Name $win_output\dev -ItemType Directory
    New-Item -Name $win_output\test_suite -ItemType Directory
}

Remove-Item -Recurse -Force build
New-Item -Name build -ItemType Directory
Set-Location -Path build

If ($IsWindows) {
    cmake -S .. -B . 
    msbuild the_deck.sln -p:Configuration=Release
    Copy-Item decky\Release\* -Destination ..\$win_output\dev
    Copy-Item ..\decky\the_deck.h -Destination ..\$win_output\dev
    Copy-Item bin\Release\gtest.dll -Destination ..\$win_output\test_suite
    Copy-Item bin\Release\gtest_main.dll -Destination ..\$win_output\test_suite
    Copy-Item tests\Release\decky_gtest.exe -Destination ..\$win_output\test_suite
    Set-Location -Path ..
    Compress-Archive -Path=$win_output -DestinationPath=$win_output.zip
}
Else { 
    cmake -S .. -B . -G Ninja -D CMAKE_BUILD_TYPE=Release
    ninja
    sudo ninja install
    Set-Location -Path ..
}
Remove-Item -Recurse -Force build