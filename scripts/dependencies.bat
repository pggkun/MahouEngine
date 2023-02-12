set SDL_VERSION=2.0.22
set GLM_VERSION=0.9.9.8
set FREETYPE_VERSION=2.12.1
set GLEW_VERSION=2.1.0
set IMGUI_VERSION=1.89
@REM set LUA_VERSION=5.4.4

powershell -Command "(New-Object Net.WebClient).DownloadFile('https://www.libsdl.org/release/SDL2-%SDL_VERSION%.tar.gz', 'SDL2-%SDL_VERSION%.tar.gz')"
tar -xvf SDL2-%SDL_VERSION%.tar.gz
mkdir %~dp0SDL2-%SDL_VERSION%\build
cd %~dp0SDL2-%SDL_VERSION%\build
cmake ../
cmake --build .
cd ../../

mkdir %~dp0deps\SDL2\lib
move %~dp0SDL2-%SDL_VERSION%\build\Debug\SDL2d.lib %~dp0deps\SDL2\lib\SDL2.lib
move %~dp0SDL2-%SDL_VERSION%\build\Debug\SDL2maind.lib %~dp0deps\SDL2\lib\SDL2main.lib
move %~dp0SDL2-%SDL_VERSION%\build\Debug\SDL2-staticd.lib %~dp0deps\SDL2\lib\SDL2-static.lib
move %~dp0SDL2-%SDL_VERSION%\build\Debug\SDL2d.dll %~dp0deps\SDL2\lib\SDL2d.dll

mkdir %~dp0deps\SDL2\include\SDL2
powershell -Command "cp SDL2*/include/* deps/SDL2/include/SDL2/"
del /s %~dp0SDL*.tar.gz
@rd /S /Q %~dp0SDL2-%SDL_VERSION%

powershell -Command "(New-Object Net.WebClient).DownloadFile('https://github.com/g-truc/glm/archive/refs/tags/%GLM_VERSION%.tar.gz', 'glm-%GLM_VERSION%.tar.gz')"
tar -xvf glm-%GLM_VERSION%.tar.gz
move %~dp0glm-%GLM_VERSION%\glm %~dp0deps\
del /s %~dp0glm*.tar.gz
@rd /S /Q %~dp0glm-%GLM_VERSION%

powershell -Command "(New-Object Net.WebClient).DownloadFile('https://github.com/nothings/stb/tarball/master', 'stb.tar.gz')"
tar -xvf stb.tar.gz
mkdir %~dp0deps\stb
powershell -Command "(cp -r *stb*/* deps/stb/)"
del /s %~dp0stb.tar.gz
powershell -Command "(rm -r *stb*)"

powershell -Command "(New-Object Net.WebClient).DownloadFile('https://download.savannah.gnu.org/releases/freetype/freetype-%FREETYPE_VERSION%.tar.gz', 'freetype-%FREETYPE_VERSION%.tar.gz')"
tar -xvf freetype-%FREETYPE_VERSION%.tar.gz
mkdir %~dp0freetype-%FREETYPE_VERSION%\build
cd %~dp0freetype-%FREETYPE_VERSION%\build
cmake ../
cmake --build .
cd ../../

mkdir %~dp0deps\FreeType\lib
move %~dp0freetype-%FREETYPE_VERSION%\build\Debug\freetyped.lib  %~dp0deps\FreeType\lib\freetype.lib
move %~dp0freetype-%FREETYPE_VERSION%\include %~dp0deps\FreeType\
del /s %~dp0freetype*.tar.gz
@rd /S /Q %~dp0freetype-%FREETYPE_VERSION%

@REM powershell -Command "(New-Object Net.WebClient).DownloadFile('https://sourceforge.net/projects/glew/files/glew/%GLEW_VERSION%/glew-%GLEW_VERSION%.tgz/download', 'glew-%GLEW_VERSION%.tar.gz')"
@REM tar -xvf glew-%GLEW_VERSION%.tar.gz
@REM mkdir %~dp0glew-%GLEW_VERSION%\build\cmake\build
@REM cd %~dp0glew-%GLEW_VERSION%\build\cmake\build
@REM cmake ../
@REM cmake --build .
@REM cd ../../../../

@REM mkdir %~dp0deps\glew\lib\Release\Win32
@REM move %~dp0glew-%GLEW_VERSION%\build\cmake\build\lib\Debug\glew32d.lib  %~dp0deps\glew\lib\Release\Win32\glew32.lib
@REM powershell -Command "cp -r glew-%GLEW_VERSION%\include\GL deps\glew\include\gl"
@REM del /s %~dp0glew*.tar.gz
@REM @rd /S /Q %~dp0glew-%GLEW_VERSION%

powershell -Command "(New-Object Net.WebClient).DownloadFile('https://sourceforge.net/projects/glextwin32/files/glext.zip/download', 'glext.zip')"
powershell -Command "Expand-Archive glext.zip -DestinationPath glext"
mkdir %~dp0deps
move %~dp0glext\glext  %~dp0deps\
del /s glext.zip
@rd /S /Q %~dp0glext

powershell -Command "(New-Object Net.WebClient).DownloadFile('https://sourceforge.net/projects/luabinaries/files/5.4.2/Windows%%20Libraries/Static/lua-5.4.2_Win32_vc14_lib.zip/download', 'lua.zip')"
powershell -Command "Expand-Archive lua.zip -DestinationPath lua"
mkdir %~dp0deps\lua\lib
move %~dp0lua\lua54.lib  %~dp0deps\lua\lib\lua.lib
move %~dp0lua\include %~dp0deps\lua\
del /s lua.zip
@rd /S /Q %~dp0lua

powershell -Command "(New-Object Net.WebClient).DownloadFile('https://github.com/ocornut/imgui/tarball/v%IMGUI_VERSION%', 'imgui.tar.gz')"
tar -xvf imgui.tar.gz
mkdir %~dp0deps\ImGUI
powershell -Command "cp *imgui*/*.h deps/ImGUI/"
powershell -Command "cp *imgui*/*.cpp deps/ImGUI/"
powershell -Command "cp *imgui*/*.hpp deps/ImGUI/"

powershell -Command "cp *imgui*/backends/*sdl* deps/ImGUI/"
powershell -Command "cp *imgui*/backends/*opengl* deps/ImGUI/"
del /s imgui.tar.gz
powershell -Command "rm -r *imgui*"

powershell -Command "cp -r deps ../deps"
powershell -Command "rm -r deps"
