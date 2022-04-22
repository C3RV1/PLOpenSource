CC = C:/MinGW/bin/g++.exe

SRCS = test.cpp ./k4sdl/*.cpp ./k4sdl/input/*.cpp ./k4sdl/sprite/*.cpp ./k4sdl/font/*.cpp ./k4sdl/Vector2/*.cpp

INCLUDE_PATHS= -IC:/dev/sdl2/SDL2-2.0.20/i686-w64-mingw32/include/SDL2 -IC:/dev/SDL2_image-2.0.5/i686-w64-mingw32/include/SDL2 -IC:/dev/SDL2_ttf-2.0.18/i686-w64-mingw32/include/SDL2

LIBRARY_PATHS= -LC:/dev/sdl2/SDL2-2.0.20/i686-w64-mingw32/lib -LC:/dev/SDL2_image-2.0.5/i686-w64-mingw32/lib -LC:/dev/SDL2_ttf-2.0.18/i686-w64-mingw32/lib

COMPILER_FLAGS= -g -fdiagnostics-color=always

LINKER_FLAGS= -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

OUT = ./build/output.exe

all: $(OUT)

$(OUT):
	$(CC) $(COMPILER_FLAGS) $(SRCS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OUT)
