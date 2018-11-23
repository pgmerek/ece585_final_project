CC=g++
CFLAGS=-I.
DEPS=header.h

%.o: %.cpp $(DEPS)
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

cache_simulator: main.o cache.o set.o entry.o traces.o
	$(CC) -o cache_simulator.out *.o

clean:
	rm -f *.o *.out 

