FLAGS = -g -Wall

all: shell tcp-server tcp-client

main: main.o lib_shell.a shell.h
	gcc  -o main main.o lib_shell.a 

main.o: main.c shell.h
	gcc  -c main.c

shell.o: shell.c shell.h
	gcc -c shell.c

shell: shell.o lib_shell.a shell.h
	gcc -o shell shell.o lib_shell.a

lib_shell.a: shell.o
	ar -rcs lib_shell.a shell.o

tcp-server: tcp-server.c
	gcc  -o server tcp-server.c

tcp-client: tcp-client.c
	gcc  -o client tcp-client.c

clean:
	rm -f *.o *.a server client main shell

runs:
	./server

runc:
	./client

runs-strace:
	strace -f ./server

runc-strace:
	strace -f ./client