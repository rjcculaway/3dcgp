LDFLAGS  :=$(shell sdl2-config --libs) -lm
CXXFLAGS :=$(shell sdl2-config --cflags)
UPNG :=upng/upng.c
build:
	clang -Wall -std=c99 ${CXXFLAGS} src/*.c ${UPNG} ${LDFLAGS} -o renderer
run:
	./renderer
clean:
	rm renderer