all:vibora 

vibora: vibora.c
	gcc -Wall -o vibora vibora.c -lncurses

clean:
	rm vibora
