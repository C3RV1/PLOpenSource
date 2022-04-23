cmake_minimum_required(VERSION 3.10)
project(PLOpenSource VERSION 0.1.0)

set(SDL2_INCLUDE_DIRS "C:/dev/sdl2/SDL2-2.0.20/i686-w64-mingw32/include/SDL2;C:/dev/SDL2_image-2.0.5/i686-w64-mingw32/include/SDL2;C:/dev/SDL2_ttf-2.0.18/i686-w64-mingw32/include/SDL2;C:/dev/SDL2_mixer-2.0.4/i686-w64-mingw32/include/SDL2")
set(SDL2_LIBRARIES "C:/dev/sdl2/SDL2-2.0.20/i686-w64-mingw32/lib;C:/dev/SDL2_image-2.0.5/i686-w64-mingw32/lib;C:/dev/SDL2_ttf-2.0.18/i686-w64-mingw32/lib;C:/dev/SDL2_mixer-2.0.4/i686-w64-mingw32/lib")

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/output)

include_directories(${SDL2_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARIES})

add_executable(PLOpenSource main.cpp)
add_subdirectory(k4sdl)
add_subdirectory(game)
target_link_libraries(PLOpenSource SDL2 SDL2_image SDL2_ttf SDL2_mixer)
