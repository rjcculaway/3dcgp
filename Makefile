LDFLAGS  :=$(shell sdl2-config --libs) -lm
CXXFLAGS :=$(shell sdl2-config --cflags)
build:
	clang -Wall -std=c99 ${CXXFLAGS} src/*.c ${LDFLAGS} -o renderer
run:
	./renderer
clean:
	rm renderer