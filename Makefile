sdltest:	sdltest.cpp
	g++ `sdl-config --cflags --libs` sdltest.cpp -o sdltest

clean:
	rm sdltest
