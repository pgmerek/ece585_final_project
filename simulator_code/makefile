CC = g++

cache_simulator: main.o cache.o set.o entry.o traces.o
	$(CC) -Wall -g *.cpp
	$(CC) -o cache_simulator.out *.o

clean:
	rm -f *.o cache_simulator.out  

