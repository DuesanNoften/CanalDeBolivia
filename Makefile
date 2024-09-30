CC = gcc
CFLAGS = -Wall -g   
#----*****Change the pthread for our thread library*****-----
LIBS = -lSDL2 -lpthread   # SDL2 & pthread libraries


TARGET = canal_bolivia # Exe name
OBJS = main.o ship.o # canal.o

# compile all
all: $(TARGET)

# compile exe from .o
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# compile main.c
main.o: main.c ship.h # canal.h
	$(CC) $(CFLAGS) -c main.c

# compile ship.c
ship.o: ship.c ship.h
	$(CC) $(CFLAGS) -c ship.c

# compile canal.c
#canal.o: canal.c canal.h ship.h
#	$(CC) $(CFLAGS) -c canal.c

# clean compiled files
clean:
	rm -f $(OBJS) $(TARGET)
