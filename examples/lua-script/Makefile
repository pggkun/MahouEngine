GAME_NAME 	:= LuaScript
CC 		  	:= g++ -std=c++14
ROOTDIR 	:= $(CURDIR)/../../
LDFLAGS		:= -L$(ROOTDIR)build/lib -L$(ROOTDIR)deps/SDL2/lib -L$(ROOTDIR)deps/FreeType/lib -L$(ROOTDIR)deps/lua/lib -L$(ROOTDIR)deps/glew/lib/Release/Win32
IFLAGS		:= -I$(ROOTDIR)build/include \
			   -I$(CURDIR) -I$(CURDIR)/resources -I$(ROOTDIR)deps/FreeType/include\
			   -I$(ROOTDIR)build/include/render -I$(ROOTDIR)build/include/render/basic_shaders -I$(ROOTDIR)build/include/GLEW \
			   -I$(ROOTDIR)deps/stb -I$(ROOTDIR)deps/SDL2/include -I$(ROOTDIR)deps/glm -I$(ROOTDIR)deps/lua/include
LDLIBS 	  	:= -w -lMahouEngine -llua -lmingw32 -lSDL2main -lSDL2 -lfreetype -lopengl32 -lglu32

.PHONY: build, clean, res

build:
	mkdir -p $(CURDIR)/build
	$(CC) $(CURDIR)/*.cpp $(IFLAGS) $(LDFLAGS) $(LDLIBS) -o $(CURDIR)/build/$(GAME_NAME)

clean:
	rm -rf $(CURDIR)/build
	rm -rf $(CURDIR)/resources/*.h

res:
	for i in resources/*.png;            					\
    do                                  								\
            python $(ROOTDIR)tools/PGGKBin2h.py $${i} $${i/.png/_png.h};  \
    done