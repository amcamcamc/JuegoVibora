#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "controles.h"
#include "vibora.h"
#include "escenario.h"
#include "juego.h"

#define COLOR_RESET   "\x1b[0m"

Escenario escenarioActual;

Vibora *listaViboras[1];
Vibora *viboraJugador;
Vibora *viboraComputadora;

Elemento *manzana;

//Configuracion del juego
int juegoActivo = 0;
int juegoVelocidad = 100000;
int nivel = 0;

//Datos ncurses
int maxX = 0;
int maxY = 0;

void escucharInput_jugador()
{
	int tecla = getch();
	
	if (compararAccionTecla(tecla, MOVER_IZQUIERDA))
	{ viboraJugador->direccion = redireccionarIzquierda(viboraJugador->direccion); }
	if (compararAccionTecla(tecla, MOVER_DERECHA))
	{ viboraJugador->direccion = redireccionarDerecha(viboraJugador->direccion); }
	if (compararAccionTecla(tecla, REGRESAR))
	{ juegoActivo = 0; }
}

void aparecerManzana(Elemento *manzanaNueva, int posX, int posY)
{
	manzanaNueva->x = posX;
    manzanaNueva->y = posY;
    manzanaNueva->dibujo = "a";
}

void dibujarManzana(Elemento *manzanaObj)
{
	attron(COLOR_PAIR(7));
	mvprintw(manzanaObj->y, manzanaObj->x, manzanaObj->dibujo);
}

void actualizarViboras()
{
	for (int i = 0; i < 2; i++)
	{
		moverVibora(listaViboras[i], maxX, maxY);
		attron(COLOR_PAIR(i+1));
		dibujarVibora(listaViboras[i]);
	}
}

void inicializarJuego(char* nivelSeleccionado, int velocidadSeleccionada)
{
	init_pair(1, COLOR_BLACK, COLOR_BLUE); //vibora 1
	init_pair(2, COLOR_BLACK, COLOR_YELLOW); //vibora 2
	
	init_pair(5, COLOR_WHITE, COLOR_GREEN); //pasto
	init_pair(6, COLOR_WHITE, COLOR_BLACK); //pared
	init_pair(7, COLOR_WHITE, COLOR_RED); //manzana
	
	initEscenario(&escenarioActual);
	cargarEscenario(&escenarioActual, nivelSeleccionado);
	
	viboraJugador = crearVibora(NULL, 1, (maxX/2)+5, maxY/2, arriba);
	viboraComputadora = crearVibora(NULL, 1, (maxX/2)-5, maxY/2, abajo);
	
	if (viboraJugador != NULL)
	{
		listaViboras[0] = viboraJugador;
		listaViboras[1] = viboraComputadora;
	}
	
	manzana = crearElemento(-1,-1, NULL);
	aparecerManzana(manzana, maxX/2, maxY/2);
}

void actualizarJuego()
{
	//getmaxyx(stdscr, maxY, maxX);
	
	clear(); //limpia la pantalla
	
	imprimirEscenario(&escenarioActual, maxX, maxY);
	dibujarManzana(manzana);
	actualizarViboras();
	escucharInput_jugador();
		
	refresh(); //actualiza ncurses
		
	usleep(juegoVelocidad); //retrasa el programa
}

int jugarJuego()
{
	inicializarJuego("nivel1.txt", 50000);
	while (juegoActivo)
	{
		actualizarJuego();
	}
	return juegoActivo;
} 
