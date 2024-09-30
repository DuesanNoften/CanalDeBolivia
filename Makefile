CC = gcc
CFLAGS = -Wall -g
LIBS = -lSDL2
TARGET = canal_bolivia

all: $(TARGET)

$(TARGET): main.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o $(TARGET)
