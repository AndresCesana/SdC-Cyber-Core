CC = gcc
ASM = nasm
CFLAGS = -Wall -O2 -m32 -fno-pie -no-pie
LDFLAGS = -lcurl -lm -lcjson 

all: main

main: main.c suma.o
	$(CC) $(CFLAGS) -g -o main main.c suma.o $(LDFLAGS)

suma.o: suma.asm
	$(ASM) -f elf32 suma.asm -o suma.o

clean:
	rm -f main suma.o gmon.out
