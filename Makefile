RESOURCES := resources
TOOLS	  := tools
BUILD	  := build
SOURCE	  := src
ENGINE	  := engine
EXAMPLES  := $(sort $(dir $(wildcard $(CURDIR)/examples/*/)))
CC 		  := g++ -std=c++14
OUTPUT    := game
LDFLAGS   := -L$(CURDIR)/deps/SDL2/lib -L$(CURDIR)/$(BUILD)/ -L$(CURDIR)/deps/FreeType/lib -L$(CURDIR)/deps/lua/lib -L$(CURDIR)/deps/glew/lib/Release/Win32
IFLAGS    := -I$(CURDIR)/deps/stb -I$(CURDIR)/deps/SDL2/include -I$(CURDIR)/deps/glm \
		  									-I$(CURDIR)/$(SOURCE) \
											-I$(CURDIR)/$(SOURCE)/render/basic_shaders \
											-I$(CURDIR)/$(SOURCE)/render -I$(CURDIR)/$(RESOURCES) -I$(CURDIR)/deps/FreeType/include \
											-I$(CURDIR)/deps/lua/include -I$(CURDIR)/deps/glew/include
LDLIBS 	  := -w -llua -lmingw32 -lSDL2main -lSDL2 -lfreetype -lopengl32 -lglu32

.PHONY: clean, res, shaders, deps, engine

engine: clean, shaders
	rm -rf $(CURDIR)/build
	mkdir -p $(CURDIR)/build
	rm -rf $(CURDIR)/$(SOURCE)/*.o
	@for cppfile in $(CURDIR)/$(SOURCE)/*.cpp;\
	do \
		$(CC) -c $${cppfile} $(IFLAGS) $(LDFLAGS) $(LDLIBS) -o $${cppfile/.cpp/.o};  \
	done; \
	cp $(CURDIR)/$(SOURCE)/*.o  $(CURDIR)/build/
	rm -rf $(CURDIR)/$(SOURCE)/*.o

	@for cpprfile in $(CURDIR)/$(SOURCE)/render/*.cpp; \
	do \
		$(CC) -c $${cpprfile} $(IFLAGS) $(LDFLAGS) $(LDLIBS) -o $${cpprfile/.cpp/.o};  \
	done; \
	cp $(CURDIR)/$(SOURCE)/render/*.o  $(CURDIR)/build/
	rm -rf $(CURDIR)/$(SOURCE)/render/*.o

	@for cfile in $(CURDIR)/$(SOURCE)/GLEW/*.c; \
	do \
		$(CC) -c $${cfile} $(IFLAGS) $(LDFLAGS) $(LDLIBS) -o $${cfile/.c/.o};  \
	done; \
	cp $(CURDIR)/$(SOURCE)/GLEW/*.o  $(CURDIR)/build/
	rm -rf $(CURDIR)/$(SOURCE)/GLEW/*.o

	mkdir -p $(CURDIR)/build/lib
	ar rcs $(CURDIR)/build/lib/libMahouEngine.a  $(CURDIR)/build/*.o
	rm -rf $(CURDIR)/build/*.o

	mkdir -p $(CURDIR)/build/include/render/basic_shaders $(CURDIR)/build/include/GLEW
	cp $(CURDIR)/$(SOURCE)/*.h $(CURDIR)/build/include/
	cp $(CURDIR)/$(SOURCE)/render/*.h $(CURDIR)/build/include/render/
	cp $(CURDIR)/$(SOURCE)/render/basic_shaders/*.h $(CURDIR)/build/include/render/basic_shaders
	cp $(CURDIR)/$(SOURCE)/GLEW/*.h $(CURDIR)/build/include/GLEW

clean:
	@echo $(EXAMPLES)
	-$(foreach var,$(EXAMPLES), rm -rd $(var)/$(BUILD);)
	-$(foreach var,$(EXAMPLES), rm -rf $(var)/$(RESOURCES)/*_png.h;)
	rm -rf ${SOURCE}/render/basic_shaders/*.h
	rm -rf $(CURDIR)/build

shaders:
	@for shader in ${SOURCE}/render/basic_shaders/*.shader;\
	do \
		python $(CURDIR)/$(TOOLS)/PGGKBin2h.py $${shader} $${shader/.shader/_shader.h};  \
	done

res: clean, shaders
	@for entry in ${EXAMPLES};              								\
    do                                      								\
        for i in $${entry}$(RESOURCES)/*.png;            					\
        do                                  								\
            python $(CURDIR)/$(TOOLS)/PGGKBin2h.py $${i} $${i/.png/_png.h};  \
        done                                								\
    done

deps:
	shell scripts/dependencies.sh