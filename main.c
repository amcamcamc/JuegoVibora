#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ncurses.h>

#include "controles.h"
#include "juego.h"
#include "menuJuego.h"

//Codigos de salida
#define SALIDA_EXITOSA 0 //El programa sale de manera exitosa
#define SALIDA_ERROR 1 //El programa salio por un error

int enPrograma = 1;	//El usuario se encuentra dentro del programa
int enMenus = 1; //El usuario se encuentra dentro de los menus
int enJuego = 0; //El usuario se encuentra dentro del juego

// inicializa el programa y ncurses
void inicializarPrograma()
{
	srand((unsigned) time(NULL));
	
	initscr(); //inicializa la ventana
	start_color(); //empieza a utilizar color
	noecho(); //no imprimir teclas presionadas
	keypad(stdscr, TRUE); //abilita el presionar teclas
	curs_set(FALSE); //no mostrar cursor
	cbreak();
	
	getmaxyx(stdscr, maxY, maxX); //obtener tamanos maximos terminal
	
	//estas son las resoluciones por defecto para que funcione el juego
	//ya que se necesitaria hacer otros cambios para que el juego
	//se adapte a la resolucion
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
			if (enMenus == 0) { if (juegoActivo) { enJuego = 1; } } //si salimos de los menus y estamos en el juego
		}
		
		while (enJuego)
		{
			enJuego = jugarJuego();
			if (enJuego == 0) { enMenus = 1; } //si salimos del juego entonces entramos automaticamente a los menus
		}
		
		if (!enMenus && !enJuego) //si no estamos en los menus ni en el juego
		{ enPrograma = 0; } //salir
	}
	
	endwin(); //regresa la terminal a su estado normal
	nocbreak();
	
	return SALIDA_EXITOSA;
}
