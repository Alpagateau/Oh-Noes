lib = -llua -lraylib 
libdir = -L./lua54 -L./raylib/build/raylib
idir = -I./
opts = -g -static-libstdc++
filename = ./test

test : tiles.o main.o physics.o
	echo Building [test]
	g++ main.o tiles.o physics.o -o $(filename) $(libdir) $(lib) $(opts) $(idir)

main.o : main.cpp
	echo Building [main.cpp]
	g++ main.cpp -c main.o -I./ $(opts)

tiles.o : tiles.cpp 
	echo Building [tiles.cpp]
	g++ tiles.cpp -c tiles.o -I./ $(opts)

physics.o : physics.cpp 
	echo Building [physics.cpp]
	g++ physics.cpp -c physics.o -I./ $(opts) $(libdir) $(lib) $(idir)
