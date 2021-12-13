all: main
	
main: main.c menuJuego.o juego.o controles.o vibora.o rival.o escenario.o
	gcc -Wall -o main main.c menuJuego.o juego.o controles.o vibora.o rival.o escenario.o -lm -lncurses
	
menuJuego.o: menuJuego.c menuJuego.h
	gcc -Wall -c menuJuego.c -lncurses

juego.o: juego.c juego.h
	gcc -Wall -c juego.c -lncurses
	
controles.o: controles.c controles.h
	gcc -Wall -c controles.c -lncurses
	
vibora.o: vibora.c vibora.h
	gcc -Wall -c vibora.c -lncurses
	
rival.o: rival.c rival.h
	gcc -Wall -c rival.c -lncurses
	
escenario.o: escenario.c escenario.h
	gcc -Wall -c escenario.c -lncurses
	
clean:
	rm main *.o
