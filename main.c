#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <ncurses.h>

#include "controles.h"
#include "juego.h"
#include "menuJuego.h"

//Codigos de salida
#define SALIDA_EXITOSA 0 //El programa sale de manera exitosa
#define SALIDA_ERROR 1 //El programa salio por un error

int enPrograma = 1;
int enMenus = 1;
int enJuego = 0;

// inicializa el programa y ncurses
void inicializarPrograma()
{
	initscr(); //inicializa la ventana
	start_color();
	noecho(); //no imprimir teclas presionadas
	keypad(stdscr, TRUE); //abilita el presionar teclas
	curs_set(FALSE); //no mostrar cursor
	cbreak();
	
	getmaxyx(stdscr, maxY, maxX); //obtener tamanos maximos terminal
	maxY = 25;
	maxX = 80;
	
	timeout(0);
}

int main()
{
	inicializarPrograma(); 
	
	while (enPrograma)
	{
		//getmaxyx(stdscr, maxY, maxX); //por si la consola cambia tamano
		
		while (enMenus == 1)
		{
			enMenus = mostrarMenus();
			if (enMenus == 0) { if (juegoActivo) { enJuego = 1; } }
		}
		
		while (enJuego)
		{
			enJuego = jugarJuego();
			if (enJuego == 0) { enMenus = 1; }
		}
		
		if (!enMenus && !enJuego) //si no estamos en los menus ni en el juego
		{ enPrograma = 0; } //salir
	}
	
	endwin(); //regresa la terminal a su estado normal
	nocbreak();
	
	return SALIDA_EXITOSA;
}
