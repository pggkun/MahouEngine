powershell -Command "Get-ChildItem -Path './resources' | Foreach-Object {  python ../../tools/PGGKBin2h.py $_.FullName $_.FullName.Replace('.png', '_png.h') }"
powershell -Command "rm -r tmp"
powershell -Command "rm -r build"
mkdir %~dp0tmp
cd %~dp0tmp
cmake ../
cmake --build .
cd ../

mkdir %~dp0build
powershell -Command "cp tmp/Debug/*.exe build/"
powershell -Command "cp ../../deps/glext/lib/glext.dll build/"
powershell -Command "cp ../../deps/SDL2/lib/SDL2d.dll build/"
powershell -Command "rm -r tmp"