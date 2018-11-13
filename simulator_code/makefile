#/*CC = g++

#cache_simulator: main.cpp cache.cpp set.cpp tag.cpp
#	$(CC) -Wall -g -o main.o main.cpp cache.cpp set.cpp tag.cpp header.h

#stevDev: main.cpp
#	$(CC) -Wall -g -o main.o main.cpp 

#clean:
#	rm main.o cache_simulator.out  



# specify all source files here
SRCS = main.cpp cache.cpp set.cpp set.cpp tag.cpp header.h
# specify target here (name of executable)
TARG = cache_simulator
# specify compiler, compile flags, and needed libs
CC   = g++
OPTS = -Wall -O -g
LIBS = -lm
# this translates .c files in src list to .o’s
OBJS = $(SRCS:.cpp=.o) 
#OBJS2= $(SRCS2:.cpp=.o)
# all is not really needed, but is used to generate the target
all: $(TARG) #$(TARG2)
# this generates the target executable
$(TARG): $(OBJS)
	$(CC) -o $(TARG) $(OBJS) $(LIBS)

#$(TARG2): $(OBJS2)
#	$(CC) -o $(TARG2) $(OBJS2) $(LIBS)
# this is a generic rule for .o files
%.o: %.cpp
	$(CC) $(OPTS) -c $< -o $@
# and finally, a clean line
clean:
	rm -f $(OBJS) $(TARG)#$(TARG2) $(OBJS2)

