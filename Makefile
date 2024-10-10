CC = gcc
CFLAGS = -Wall -g   
#----*****Change the pthread for our thread library*****-----
LIBS = -lSDL2 -lpthread   # SDL2 & pthread libraries
TARGET = canal_bolivia # Exe name
OBJS = main.o ship.o canal.o shipGenerator.o

# compile all
all: $(TARGET)

# compile exe from .o
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# compile main.c
main.o: main.c ships/ship.h canal.h ships/shipGenerator.h
	$(CC) $(CFLAGS) -c main.c

# compile ship.c
ship.o: ships/ship.c ships/ship.h
	$(CC) $(CFLAGS) -c ships/ship.c

# compile canal.c
canal.o: canal.c canal.h ships/ship.h
	$(CC) $(CFLAGS) -c canal.c

# compile shipGenerator.c
shipGenerator.o: ships/shipGenerator.c ships/shipGenerator.h ships/ship.h
	$(CC) $(CFLAGS) -c ships/shipGenerator.c

# clean compiled files
clean:
	rm -f $(OBJS) $(TARGET)
