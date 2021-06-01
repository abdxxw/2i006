C_FLAGS = -g -Wall -lm

all: main mainHash mainTest

entree_sortie.o : entree_sortie.c
	gcc $(C_FLAGS) -c entree_sortie.c

biblio.o : biblio.c
	gcc $(C_FLAGS) -c biblio.c

biblioHash.o : biblioHash.c
	gcc $(C_FLAGS) -c biblioHash.c

main.o : main.c
	gcc $(C_FLAGS) -c main.c


main	:  biblio.o entree_sortie.o main.o
	gcc $(C_FLAGS) -o main main.o biblio.o entree_sortie.o 

mainHash.o	: mainHash.c
	gcc $(C_FLAGS) -c mainHash.c

mainHash	:  mainHash.o biblioHash.o entree_sortie.o 
	gcc $(C_FLAGS) -o mainHash mainHash.o biblioHash.o entree_sortie.o 


mainTest.o : mainTest.c
	gcc $(C_FLAGS) -c mainTest.c

mainTest	:  biblio.o biblioHash.o entree_sortie.o mainTest.o
	gcc $(C_FLAGS) -o mainTest biblio.o biblioHash.o entree_sortie.o mainTest.o

clean	:
	rm -f *.o main