CC = gcc -o main -masm=intel main.c -msse4.2

all:
	$(CC)
debug:
	$(CC) -g
asm:
	gcc -masm=intel main.c -msse4.2 -S -o main.asm -O3
clean:
	rm main