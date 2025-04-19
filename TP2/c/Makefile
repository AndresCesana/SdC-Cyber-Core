# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lcurl -lcjson

# Target executable
TARGET = main

# Source files
SRCS = main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Build the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean