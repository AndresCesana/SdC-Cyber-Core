CC = gcc
ASM = nasm
CFLAGS = -Wall -O2 -m64
LDFLAGS = -lcurl -lcjson

all: gini

gini: main.c gini.o
	$(CC) $(CFLAGS) -no-pie -o gini main.c gini.o $(LDFLAGS)

gini.o: gini.asm
	$(ASM) -f elf64 gini.asm -o gini.o

clean:
	rm -f gini gini.o
