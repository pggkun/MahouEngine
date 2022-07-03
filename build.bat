powershell -Command "rm -r tmp"
powershell -Command "rm -r build"
mkdir %~dp0tmp
cd %~dp0tmp
cmake ../
cmake --build .
cd ../

mkdir %~dp0build\lib %~dp0build\include\render\basic_shaders
powershell -Command "cp -r tmp/Debug/* build/lib/"
powershell -Command "cp -r src/*.h build/include/"
powershell -Command "cp -r src/render/*.h build/include/render/"
powershell -Command "cp -r src/render/basic_shaders/*.h build/include/render/basic_shaders/"

powershell -Command "rm -r tmp"