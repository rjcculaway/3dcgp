LDFLAGS  :=$(shell sdl2-config --libs)
CXXFLAGS :=$(shell sdl2-config --cflags)
build:
	clang -Wall -std=c99 ${CXXFLAGS} src/main.c ${LDFLAGS} -o renderer
run:
	./renderer
clean:
	rm renderer