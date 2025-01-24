Remove-Item -Recurse -Force build
Remove-Item -Recurse -Force artifacts
New-Item -Name build -ItemType Directory
New-Item -Name artifacts -ItemType Directory
New-Item -Name artifacts\dev -ItemType Directory
New-Item -Name artifacts\gtest -ItemType Directory
Set-Location -Path build
cmake -S .. -B . 
msbuild the_deck.sln -p:Configuration=Release
Copy-Item decky\Release\* -Destination ..\artifacts\dev
Copy-Item ..\decky\the_deck.h -Destination ..\artifacts\dev
Copy-Item bin\Release\gtest.dll -Destination ..\artifacts\gtest
Copy-Item bin\Release\gtest_main.dll -Destination ..\artifacts\gtest
Copy-Item tests\Release\decky_gtest.exe -Destination ..\artifacts\gtest
Set-Location -Path ..
Remove-Item -Recurse -Force build