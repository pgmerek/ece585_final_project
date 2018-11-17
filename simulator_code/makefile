CC = g++

cache_simulator: main.o cache.o set.o tag.o traces.o
	$(CC) -Wall -g *.cpp
	$(CC) -o cache_simulator *.o

clean:
	rm -f *.o cache_simulator.out  

