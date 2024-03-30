CC = g++
CFLAGS = -Wall -Iraylib -Iengine
LDFLAGS = -Lraylib/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all : run

main.o: src/main.cpp
	$(CC) -c src/main.cpp $(CFLAGS) $(LDFLAGS) -o bin/main.o

object.o: engine/object.cpp
	$(CC) -c engine/object.cpp $(CFLAGS) $(LDFLAGS) -o bin/object.o

util.o: engine/util.cpp
	$(CC) -c engine/util.cpp $(CFLAGS) $(LDFLAGS) -o bin/util.o

scene.o: engine/scene.cpp
	$(CC) -c engine/scene.cpp $(CFLAGS) $(LDFLAGS) -o bin/scene.o

build: main.o object.o util.o scene.o
	$(CC) -o bin/game bin/main.o bin/object.o bin/util.o bin/scene.o $(CFLAGS) $(LDFLAGS)

run: build clean
	./bin/game

clean:
	rm -f bin/*.o
