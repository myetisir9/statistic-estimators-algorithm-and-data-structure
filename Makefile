
CC=g++
CPPFLAGS=-O2
EXE=homework2
all: main

main: main.cpp
	$(CC) $(CPPFLAGS) main.cpp -o $(EXE)

run:
	./$(EXE)

clean:
	rm $(EXE)