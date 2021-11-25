CC=g++
CPPFLAGS=-std=c++11 #-Wall -Wextra -pedantic 
DEPS = generator.h max_flow.h
OBJ = main.o generator.o max_flow.o

all: generator

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS)

generator: $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS)

clean:
	rm -f *.o generator


