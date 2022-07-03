cmake_minimum_required(VERSION 3.20)
set(OUTPUT "MahouEngine")
set(CMAKE_C_COMPILER "g++")
project(${OUTPUT})

file(GLOB cppfiles src/*.cpp)
file(GLOB cpprfiles src/render/*.cpp)
add_library(${OUTPUT} STATIC
    ${cppfiles} ${cpprfiles})

target_link_libraries(${OUTPUT} PRIVATE "deps/SDL2/lib")
target_link_libraries(${OUTPUT} PRIVATE "deps/FreeType/lib")
target_link_libraries(${OUTPUT} PRIVATE "deps/lua/lib")
target_link_libraries(${OUTPUT} PRIVATE "deps/glext/lib")

target_include_directories(${OUTPUT} PUBLIC "src")
target_include_directories(${OUTPUT} PUBLIC "src/render")
target_include_directories(${OUTPUT} PUBLIC "src/render/basic_shaders")
target_include_directories(${OUTPUT} PUBLIC "deps/stb")
target_include_directories(${OUTPUT} PUBLIC "deps/SDL2/include")
target_include_directories(${OUTPUT} PUBLIC "deps/glm")
target_include_directories(${OUTPUT} PUBLIC "src/render/basic_shaders")
target_include_directories(${OUTPUT} PUBLIC "src/render")
target_include_directories(${OUTPUT} PUBLIC "deps/glext/include")
target_include_directories(${OUTPUT} PUBLIC "deps/lua/include")
target_include_directories(${OUTPUT} PUBLIC "deps/FreeType/include")


