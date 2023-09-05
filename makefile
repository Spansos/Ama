CC = g++
CFLAGS = -g -Wall -O2

CPP_FILES = $(wildcard src/*.cpp)
OBJ_FILES = $(patsubst src/%.cpp,obj/%.o,$(CPP_FILES))

all: bin/main

.PHONY: clean run

run: bin/main
	./bin/main

clean:
	rm ~/Documents/AmateurLang/obj/* -f
	rm ~/Documents/AmateurLang/bin/* -f

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

bin/main: $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@