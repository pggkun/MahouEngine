RESOURCES := resources
TOOLS	  := tools
BUILD	  := build
SOURCE	  := src
ENGINE	  := engine
EXAMPLES  := $(sort $(dir $(wildcard $(CURDIR)/examples/*/)))
CC 		  := g++ -std=c++14 -fpermissive
OUTPUT    := game
SHELL := /bin/bash
LDFLAGS   := -L$(CURDIR)/deps/SDL2/lib -L$(CURDIR)/$(BUILD)/ -L$(CURDIR)/deps/FreeType/lib -L$(CURDIR)/deps/lua/lib -L$(CURDIR)/deps/glew/lib/Release/Win32
IFLAGS    := -I$(CURDIR)/deps/stb -I$(CURDIR)/deps/SDL2/include -I$(CURDIR)/deps/glm \
		  									-I$(CURDIR)/$(SOURCE) \
											-I$(CURDIR)/$(SOURCE)/render/basic_shaders \
											-I$(CURDIR)/$(SOURCE)/render -I$(CURDIR)/$(RESOURCES) -I$(CURDIR)/deps/FreeType/include \
											-I$(CURDIR)/deps/lua/include -I$(CURDIR)/deps/glew/include
LDLIBS 	  := -w -llua -lSDL2main -lSDL2 -lfreetype -lGLU -lglut -lGL

.PHONY: clean, res, shaders, example, deps

example:
	@for entry in ${EXAMPLES};              																		\
    do  																											\
		mkdir -p $${entry}build;                                    												\
		$(CC) $${entry}*.cpp $(CURDIR)/$(SOURCE)/*.cpp $(CURDIR)/$(SOURCE)/render/*.cpp $(CURDIR)/$(SOURCE)/GLEW/*.c\
		$(IFLAGS) -I$${entry}/resources -I$${entry} $(LDFLAGS) $(LDLIBS) -o $${entry}build/game.exe;				\
    done

clean:
	@echo $(EXAMPLES)
	-$(foreach var,$(EXAMPLES), rm -rd $(var)/$(BUILD);)
	-$(foreach var,$(EXAMPLES), rm -rf $(var)/$(RESOURCES)/*_png.h;)
	rm -rf ${SOURCE}/render/basic_shaders/*.h

shaders:
	@for shader in ${SOURCE}/render/basic_shaders/*.shader;\
	do \
		python3 $(CURDIR)/$(TOOLS)/PGGKBin2h.py $${shader} $${shader/.shader/_shader.h};  \
	done

res: clean, shaders
	@for entry in ${EXAMPLES};              								\
    do                                      								\
        for i in $${entry}$(RESOURCES)/*.png;            					\
        do                                  								\
            python3 $(CURDIR)/$(TOOLS)/PGGKBin2h.py $${i} $${i/.png/_png.h};  \
        done                                								\
    done

deps:
	shell scripts/dependencies.sh

bullet-hell : $(CURDIR)/examples/bullet-hell/*.cpp $(CURDIR)/$(SOURCE)/*.cpp $(CURDIR)/$(SOURCE)/render/*.cpp $(CURDIR)/$(SOURCE)/GLEW/*.c
	mkdir -p $(CURDIR)/examples/bullet-hell/build;                                    																				\
	$(CC) $(CURDIR)/examples/bullet-hell/*.cpp $(CURDIR)/$(SOURCE)/*.cpp $(CURDIR)/$(SOURCE)/render/*.cpp $(CURDIR)/$(SOURCE)/GLEW/*.c 								\
	$(IFLAGS) -I$(CURDIR)/examples/bullet-hell/resources -I$(CURDIR)/examples/bullet-hell $(LDFLAGS) $(LDLIBS) -o $(CURDIR)/examples/bullet-hell/build/game.exe;	\

sprite-render : $(CURDIR)/examples/sprite-render/*.cpp $(CURDIR)/$(SOURCE)/*.cpp $(CURDIR)/$(SOURCE)/render/*.cpp $(CURDIR)/$(SOURCE)/GLEW/*.c
	mkdir -p $(CURDIR)/examples/sprite-render/build;                                    																				\
	$(CC) $(CURDIR)/examples/sprite-render/*.cpp $(CURDIR)/$(SOURCE)/*.cpp $(CURDIR)/$(SOURCE)/render/*.cpp $(CURDIR)/$(SOURCE)/GLEW/*.c  								\
	$(IFLAGS) -I$(CURDIR)/examples/sprite-render/resources -I$(CURDIR)/examples/sprite-render $(LDFLAGS) $(LDLIBS) -o $(CURDIR)/examples/sprite-render/build/game.exe;	\

lua-script : $(CURDIR)/examples/lua-script/*.cpp $(CURDIR)/$(SOURCE)/*.cpp $(CURDIR)/$(SOURCE)/render/*.cpp $(CURDIR)/$(SOURCE)/GLEW/*.c
	mkdir -p $(CURDIR)/examples/lua-script/build;                                    																			\
	$(CC) $(CURDIR)/examples/lua-script/*.cpp $(CURDIR)/$(SOURCE)/*.cpp $(CURDIR)/$(SOURCE)/render/*.cpp $(CURDIR)/$(SOURCE)/GLEW/*.c 							\
	$(IFLAGS) -I$(CURDIR)/examples/lua-script/resources -I$(CURDIR)/examples/lua-script $(LDFLAGS) $(LDLIBS) -o $(CURDIR)/examples/lua-script/build/game.exe;	\
