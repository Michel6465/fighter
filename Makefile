# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g -pedantic -Wextra
LIBS = -lm
DEPFLAGS = -MMD -MP
CFLAGS += $(DEPFLAGS)

# SDL2 flags
SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LDFLAGS = $(shell sdl2-config --libs) -lSDL2_ttf -lSDL2_image -lSDL2_mixer

# Target executable
TARGET = a.out
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEP = $(SRCS:.c=.d)

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(SDL2_LDFLAGS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) -c $< -o $@

-include $(DEP)

start:
	./$(TARGET)

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
