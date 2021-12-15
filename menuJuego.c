#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "controles.h"
#include "juego.h"
#include "menu.h"

enum Menus { principal, jugar, salir };

enum Menus menuActual = principal;
int opcionActual = 0; //Opcion actual seleccionada en el menu actual
int numOpciones = 0; //Numero de opciones en el menu actual (0 incluyente)

int seguirMenus = 1; //Si se salio de los menus para entrar al juego, poner un 0

WINDOW *menuPrincipal = NULL; 
WINDOW *menuNiveles = NULL; 
WINDOW *menuPuntajes = NULL; 
WINDOW *menuAjustes = NULL; 

char opciones_menuPrincipal[][32] = {
										{"JUGAR"},
										//{"PUNTAJES"},
										//{"AJUSTES"},
										{"SALIR"},
								    };
								    
char opciones_menuJugar[][32] = {
									{"NIVEL 1"},
									{"NIVEL 2"},
									{"NIVEL 3"},
									{"VOLVER"},
								};

char opciones_menuPuntajes[][32] = {
										{"VOLVER"},
								   };
								
char opciones_menuAjustes[][32] = {
										{"VELOCIDAD: NORMAL"},
										{"JUGADOR IA"},
								  };

void renderizarOpciones(WINDOW *menu, char opciones[][32])
{		
	for (int i = 0; i <= numOpciones; i++)
	{
		mvwprintw(menu, i+2, 2, opciones[i]);
		if (opcionActual == i)
		{
			wattron(menu, A_STANDOUT);
			mvwprintw(menu, i+2, 2, opciones[i]);
			wattroff(menu, A_STANDOUT);
		}
	}
}

void escucharInput_menuPrincipal()
{
	int tecla = getch();
	
	if (compararAccionTecla(tecla, MOVER_ARRIBA))
	{ 
		if (opcionActual > 0) { opcionActual--; }
	}
	if (compararAccionTecla(tecla, MOVER_ABAJO))
	{
		if (opcionActual < numOpciones) { opcionActual++; } 
	}
	if (compararAccionTecla(tecla, ACEPTAR))
	{ 
		if (opcionActual == 0) { menuActual = jugar; }
		if (opcionActual == 1) { seguirMenus = 0; menuActual = salir; }
		menuPrincipal = NULL;
		menuNiveles = NULL;
		menuPuntajes = NULL;
		menuAjustes = NULL;
	}
	if (compararAccionTecla(tecla, REGRESAR))
	{
		seguirMenus = 0;
		menuActual = salir;
		menuPrincipal = NULL;
		menuNiveles = NULL;
		menuPuntajes = NULL;
		menuAjustes = NULL;
	}
}

void escucharInput_menuNiveles()
{
	int tecla = getch();
	if (compararAccionTecla(tecla, MOVER_ARRIBA))
	{ 
		if (opcionActual > 0) { opcionActual--; }
	}
	if (compararAccionTecla(tecla, MOVER_ABAJO))
	{
		if (opcionActual < numOpciones) { opcionActual++; } 
	}
	if (compararAccionTecla(tecla, ACEPTAR))
	{ 
		if (opcionActual == 0) { seguirMenus = 0; nivel = 1; juegoActivo = 1; menuActual = salir; }
		if (opcionActual == 1) { seguirMenus = 0; nivel = 2; juegoActivo = 1; menuActual = salir; }
		if (opcionActual == 2) { seguirMenus = 0; nivel = 3; juegoActivo = 1; menuActual = salir; }
		if (opcionActual == 3) { menuActual = principal; }
		menuPrincipal = NULL;
		menuNiveles = NULL;
		menuPuntajes = NULL;
		menuAjustes = NULL;
	}
	if (compararAccionTecla(tecla, REGRESAR))
	{
		menuActual = principal;
		menuPrincipal = NULL;
		menuNiveles = NULL;
		menuPuntajes = NULL;
		menuAjustes = NULL;
	}
}

//Muestra el menu principal del juego con todas sus opciones
void mostrarMenuPrincipal()
{	
	if (menuPrincipal == NULL)
	{
		opcionActual = 0;
		numOpciones = 1;
	
		menuPrincipal = newwin(maxY/2,maxX/2, maxY/4, maxX/4);
		box(menuPrincipal, 0,0);
	
		if (finDelJuego == 1) //Si ha ganado el jugador
		{
			mvwprintw(menuPrincipal, 0, maxX/8, "FIN: GANASTE!");
		}
		else if (finDelJuego == 2) //Si ha ganado la computadora
		{
			mvwprintw(menuPrincipal, 0, maxX/8, "FIN: TE HAS ESTRELLADO!");
		}
		else if (finDelJuego == 0) //Si se han estrellado el jugador y la computadora
		{
			mvwprintw(menuPrincipal, 0, maxX/8, "FIN: EMPATE!");
		}
		else //Si el juego no se ha iniciado todavia
		{
			mvwprintw(menuPrincipal, 0, maxX/8, "MENU PRINCIPAL");
		}
	}
	renderizarOpciones(menuPrincipal, opciones_menuPrincipal);
	
	refresh();
	wrefresh(menuPrincipal);
	
	escucharInput_menuPrincipal();
}

//Muestra el menu de escojer nivel
void mostrarMenuNiveles()
{	
	if (menuNiveles == NULL)
	{
		opcionActual = 0;
		numOpciones = 3;
		
		menuNiveles = newwin(maxY/2,maxX/2, maxY/4, maxX/4);
		box(menuNiveles, 0,0);
	
		mvwprintw(menuNiveles, 0, maxX/8, "ESCOGER NIVEL");
	}
	renderizarOpciones(menuNiveles, opciones_menuJugar);
	
	refresh();
	wrefresh(menuNiveles);
	
	escucharInput_menuNiveles();
}

//Rutina para mostrar menus hasta que se entra en el juego o se sale
int mostrarMenus()
{
	menuActual = principal;
	seguirMenus = 1;
	while (menuActual != salir)
	{
		//getmaxyx(stdscr, maxY, maxX);
		if (menuActual == principal)
		{
			mostrarMenuPrincipal();
		}
		else if (menuActual == jugar)
		{
			mostrarMenuNiveles();
		}
	}
	return seguirMenus;
}
