main: helloworld

clean:
	rm -r bin/
	mkdir -p bin/
#	find ./ -name *~* -exec rm {} +

helloworld: helloworld.o
	g++ bin/helloworld.o -lSDL -lGL -lGLU -o bin/helloworld

helloworld.o:
	g++ -c -O2 src/helloworld.cpp -o bin/helloworld.o
