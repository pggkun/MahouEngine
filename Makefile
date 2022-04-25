RESOURCES := resources
TOOLS	  := tools
BUILD	  := build
SOURCE	  := src
ENGINE	  := engine
DLL	  	  := dlls
EXAMPLES  := $(sort $(dir $(wildcard $(CURDIR)/examples/*/)))
CC 		  := g++ -std=c++14
OUTPUT    := game
LDFLAGS   := -L$(CURDIR)/deps/SDL2/lib -L$(CURDIR)/deps/glext/lib -L$(CURDIR)/$(BUILD)/ -L$(CURDIR)/deps/FreeType/lib -L$(CURDIR)/deps/lua/lib
IFLAGS    := -I$(CURDIR)/deps/stb -I$(CURDIR)/deps/SDL2/include -I$(CURDIR)/deps/glm \
		  									-I$(CURDIR)/deps/glext/include -I$(CURDIR)/$(SOURCE) \
											-I$(CURDIR)/$(SOURCE)/render -I$(CURDIR)/$(RESOURCES) -I$(CURDIR)/deps/FreeType/include \
											-I$(CURDIR)/deps/lua/include -I$(CURDIR)/$(SOURCE)/scripts
LDLIBS 	  := -w -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lopengl32 -lglu32 -lglext -lfreetype -llua54

.PHONY: clean, res, example

example:
	@for entry in ${EXAMPLES};              															\
    do  																								\
		mkdir -p $${entry}build;                                    									\
		cp -f 	$(CURDIR)/$(DLL)/*.dll	$${entry}build/;												\
		$(CC) $${entry}*.cpp $(CURDIR)/$(SOURCE)/*.cpp $(CURDIR)/$(SOURCE)/render/*.cpp 				\
		$(IFLAGS) -I$${entry}/resources -I$${entry} $(LDFLAGS) $(LDLIBS) -o $${entry}build/game.exe;	\
    done

clean:
	@echo $(EXAMPLES)
	-$(foreach var,$(EXAMPLES), rm -rd $(var)/$(BUILD);)
	-$(foreach var,$(EXAMPLES), rm -rf $(var)/$(RESOURCES)/*_png.h;)

res: clean
	@for entry in ${EXAMPLES};              								\
    do                                      								\
        for i in $${entry}$(RESOURCES)/*.png;            					\
        do                                  								\
            python $(CURDIR)/$(TOOLS)/PGGKBin2h.py $${i} $${i/.png/_png.h};  \
        done                                								\
    done

bullet-hell : $(CURDIR)/examples/bullet-hell/*.cpp $(CURDIR)/$(SOURCE)/*.cpp $(CURDIR)/$(SOURCE)/render/*.cpp
	mkdir -p $(CURDIR)/examples/bullet-hell/build;                                    																				\
	cp -f $(CURDIR)/$(DLL)/*.dll $(CURDIR)/examples/bullet-hell/build/;																								\
	$(CC) $(CURDIR)/examples/bullet-hell/*.cpp $(CURDIR)/$(SOURCE)/*.cpp $(CURDIR)/$(SOURCE)/render/*.cpp 															\
	$(IFLAGS) -I$(CURDIR)/examples/bullet-hell/resources -I$(CURDIR)/examples/bullet-hell $(LDFLAGS) $(LDLIBS) -o $(CURDIR)/examples/bullet-hell/build/game.exe;	\

sprite-render : $(CURDIR)/examples/sprite-render/*.cpp $(CURDIR)/$(SOURCE)/*.cpp $(CURDIR)/$(SOURCE)/render/*.cpp
	mkdir -p $(CURDIR)/examples/sprite-render/build;                                    																				\
	cp -f $(CURDIR)/$(DLL)/*.dll $(CURDIR)/examples/sprite-render/build/;																								\
	$(CC) $(CURDIR)/examples/sprite-render/*.cpp $(CURDIR)/$(SOURCE)/*.cpp $(CURDIR)/$(SOURCE)/render/*.cpp 															\
	$(IFLAGS) -I$(CURDIR)/examples/sprite-render/resources -I$(CURDIR)/examples/sprite-render $(LDFLAGS) $(LDLIBS) -o $(CURDIR)/examples/sprite-render/build/game.exe;	\

lua-script : $(CURDIR)/examples/lua-script/*.cpp $(CURDIR)/$(SOURCE)/*.cpp $(CURDIR)/$(SOURCE)/render/*.cpp
	mkdir -p $(CURDIR)/examples/lua-script/build;                                    																				\
	cp -f $(CURDIR)/$(DLL)/*.dll $(CURDIR)/examples/lua-script/build/;																								\
	$(CC) $(CURDIR)/examples/lua-script/*.cpp $(CURDIR)/$(SOURCE)/*.cpp $(CURDIR)/$(SOURCE)/render/*.cpp 															\
	$(IFLAGS) -I$(CURDIR)/examples/lua-script/resources -I$(CURDIR)/examples/lua-script $(LDFLAGS) $(LDLIBS) -o $(CURDIR)/examples/lua-script/build/game.exe;	\