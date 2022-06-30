SDL_VERSION=2.0.22
GLM_VERSION=0.9.9.8
GLEW_VERSION=2.1.0
FREETYPE_VERSION=2.12.1
LUA_VERSION=5.4.4

# LINUX DEPENDENCIES
sudo apt-get install libgl-dev libgl1-mesa-dev dos2unix libxext-dev git wget

# SDL2
wget https://www.libsdl.org/release/SDL2-$SDL_VERSION.tar.gz
tar -xvf SDL2-$SDL_VERSION.tar.gz
cd SDL2-$SDL_VERSION
mkdir build
cd build
../configure
make

cd ../../
mkdir -p deps/SDL2/lib
cp -r SDL2-$SDL_VERSION/build/build/.libs/* deps/SDL2/lib/

mkdir -p deps/SDL2/include/SDL2
cp -r SDL2-$SDL_VERSION/include/* deps/SDL2/include/SDL2/
rm -rf SDL2-$SDL_VERSION*

# GLM
wget https://github.com/g-truc/glm/archive/refs/tags/$GLM_VERSION.tar.gz
tar -xvf $GLM_VERSION.tar.gz
mkdir -p deps/glm
cp -r glm-$GLM_VERSION/glm/* deps/glm/
rm -rf $GLM_VERSION* glm*

# STB
wget https://github.com/nothings/stb/tarball/master
tar -xvf master
mkdir -p deps/stb
cp -r nothings-stb*/* deps/stb/
rm -rf nothings-stb* master*

# FreeType
wget https://download.savannah.gnu.org/releases/freetype/freetype-$FREETYPE_VERSION.tar.xz
tar -xvf freetype*
cd freetype-$FREETYPE_VERSION
./autogen.sh
./configure --without-harfbuzz
set -e
make

cd ../
mkdir -p deps/FreeType/lib
cp -r freetype*/objs/.libs/* deps/FreeType/lib/
cp -r freetype*/include  deps/FreeType/
rm -rf freetype*


# GLEW
wget https://sourceforge.net/projects/glew/files/glew/$GLEW_VERSION/glew-$GLEW_VERSION.tgz/download
tar -xvf download
cd glew*
set -e
make all
cd ../
mkdir -p deps/glew/lib
cp -r glew*/lib/* deps/glew/lib
rm -rf glew* download*

# LUA
wget http://www.lua.org/ftp/lua-$LUA_VERSION.tar.gz
tar -xvf lua*
cd lua-$LUA_VERSION
make generic

cd ../
mkdir -p deps/LUA/lib deps/LUA/include
cp -r lua*/src/*.a deps/LUA/lib/
cp -r lua*/src/*.h  deps/LUA/include
cp -r lua*/src/*.hpp  deps/LUA/include
rm -rf lua*