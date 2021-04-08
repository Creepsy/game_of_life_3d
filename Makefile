src = src/
build = build/
libs = libs/

game: $(src)game.cpp $(build)glad.o $(build)render_window.o
	g++ $(src)game.cpp -o game -lglfw -ldl $(build)glad.o $(build)render_window.o

$(build)glad.o: $(libs)glad/glad.h $(libs)glad/glad.c
	g++ -c $(libs)glad/glad.c -o $(build)glad.o

$(build)render_window.o: $(src)render_window.h $(src)render_window.cpp $(libs)glad/glad.h
	g++ -c $(src)render_window.cpp -o $(build)render_window.o