# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Executable name
TARGET = CanalDeBolivia

# Source files
SRCS = CEthreads/CEthread_create.c CEthreads/CEthread_join.c CEthreads/CEthread_end.c main.c CEthreads/CEmutex_init.c CEthreads/CEmutex_lock.c CEthreads/CEmutex_unlock.c CEthreads/CEmutex_destroy.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
