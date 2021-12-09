all: main
	
main: main.c menuJuego.o juego.o controles.o
	gcc -Wall -o main main.c vibora.o menuJuego.o juego.o controles.o -lm -lncurses

vibora.o: vibora.c vibora.h
	gcc -Wall -c vibora.c -lncurses
	
menuJuego.o: menuJuego.c menuJuego.h
	gcc -Wall -c menuJuego.c -lncurses

juego.o: juego.c juego.h
	gcc -Wall -c juego.c -lncurses
	
controles.o: controles.c controles.h
	gcc -Wall -c controles.c -lncurses
	
clean:
	rm  main *.o
