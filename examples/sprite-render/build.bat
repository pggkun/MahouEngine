for %%f in (%~dp0resources\*) do (
  set val=%%f
  set res=%val:.png=_png.h%
  @echo %val%
  @echo %res%
  python %~dp0../../tools/PGGKBin2h.py %val% %res%
)

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