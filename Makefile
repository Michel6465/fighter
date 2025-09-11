# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g
LIBS = -lm

# SDL2 flags
SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LDFLAGS = $(shell sdl2-config --libs) -lSDL2_ttf -lSDL2_image -lSDL2_mixer

# Target executable
TARGET = a.out

# Source files
SRCS = $(wildcard *.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(SDL2_LDFLAGS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) -c $< -o $@

start:
	./$(TARGET)

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
