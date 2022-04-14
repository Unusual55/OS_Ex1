FLAGS = -g -Wall

all: main tcp-server

main: main.o lib_shell.a shell.h
	gcc -o main main.o lib_shell.a 

main.o: main.c shell.h
	gcc -c main.c

shell.o: shell.c shell.h
	gcc -c shell.c

shell: shell.o lib_shell.a shell.h
	gcc -g -o shell shell.o lib_shell.a

lib_shell.a: shell.o
	ar -rcs lib_shell.a shell.o

tcp-server: tcp-server.c
	gcc  -o server tcp-server.c

clean:
	rm -f *.o *.a server main shell

runs:
	./server

runs-strace:
	strace -f ./server
