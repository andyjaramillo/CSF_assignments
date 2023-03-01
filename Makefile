OBJS	= main.o
SOURCE	= main.cpp
HEADER	= 
OUT	= csim
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

csim: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 


clean:
	rm -f $(OBJS) $(OUT)