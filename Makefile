#Linking with raylib and lua in a crossplatform manner
WIN_FLAGS = -static-libstdc++ -static-libgcc -lopengl32 -lgdi32 -lwinmm -Wl,-Bstatic -lstdc++ -lpthread
LIB_DIR_WIN = -L/home/martin/LIBS/raylib/win/lib  -L/home/martin/LIBS/lua/win
LIB_DIR_LINUX = -L/home/martin/LIBS/raylib/linux/lib/ -L/home/martin/LIBS/lua/linux
IDIR = -I/home/martin/LIBS/raylib/include -I/home/martin/LIBS/lua/include -I/home/martin/LIBS -I./
LIBS = -lraylib -llua54

OPTS = -g

linux: tiles.o main.o physics.o tiler.o
	g++ main.o tiles.o physics.o -o main-linux $(OPTS) $(LIBS) $(LIB_DIR_LINUX) $(IDIR)
	g++ tiler.o tiles.o -o tiler-linux               $(OPTS) $(LIBS) $(LIB_DIR_LINUX) $(IDIR)

windows: *.cpp
	x86_64-w64-mingw32-g++ *.cpp -o main-win $(IDIR) $(LIB_DIR_WIN) $(LIBS) $(WIN_FLAGS) $(OPTS)
	x86_64_w64-mingw32-g++ tiler.cpp -o tiler-linux $(OPTS) $(LIBS) $(LIB_DIR_WIN) $(IDIR) $(WIN_FLAGS)
	cp /home/martin/LIBS/lua/win/lua54.dll ./

main.o : main.cpp
	echo Building [main.cpp]
	g++ main.cpp -c main.o -I./ $(IDIR) $(OPTS)

tiles.o : tiles.cpp tiles.hpp
	echo Building [tiles.cpp]
	g++ tiles.cpp -c tiles.o -I./ $(OPTS)

physics.o : physics.cpp physics.hpp
	echo Building [physics.cpp]
	g++ physics.cpp -c physics.o -I./ $(OPTS) $(IDIR)

tiler.o : tiler.cpp tiler.hpp 
	echo Building [tiler.cpp] 
	g++ tiler.cpp -c tiler.o -I./ $(OPTS) $(IDIR)
