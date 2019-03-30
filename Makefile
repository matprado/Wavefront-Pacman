all: fila.o grafo.o pacman.o
	gcc fila.o grafo.o pacman.o -o pacman

fila.o:
	gcc -c fila.c	

grafo.o:
	gcc -c grafo.c	

pacman.o:
	gcc -c pacman.c

run:
	./pacman

clear:
	rm *.o pacman