src = src/
build = build/
libs = libs/

game: $(src)game.cpp $(build)glad.o $(libs)/stb_image.h
	g++ $(src)game.cpp -o game -lglfw -ldl $(build)glad.o

$(build)glad.o: $(libs)glad/glad.h $(libs)glad/glad.c
	g++ -c $(libs)glad/glad.c -o $(build)glad.o