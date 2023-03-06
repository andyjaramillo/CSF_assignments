OBJS	= main.o cache_funcs.o
SOURCE	= main.cpp cache_funcs.cpp
HEADER	= cache_func.h Cache.h
OUT	= csim
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

csim: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 

cache_funcs.o: cache_funcs.cpp
	$(CC) $(FLAGS) cache_funcs.cpp 


clean:
	rm -f $(OBJS) $(OUT)