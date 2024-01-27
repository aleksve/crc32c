CC = gcc -o main -masm=intel main.c -msse4.2

all:
	$(CC)
debug:
	$(CC) -g
clean:
	rm main