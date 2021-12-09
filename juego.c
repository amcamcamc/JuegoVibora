#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "controles.h"
#include "vibora.h"
#include "juego.h"

Vibora *listaViboras[1];
Vibora *viboraJugador;

//Configuracion del juego
int juegoActivo = 0;
int juegoVelocidad = 50000;
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
}

void inicializarJuego()
{
	viboraJugador = crearVibora(NULL, 1, maxX/2, maxY/2);
	
	if (viboraJugador != NULL)
	{
		listaViboras[0] = viboraJugador;
	}
}

void actualizarJuego()
{
	clear(); //limpia la pantalla
	
	for (int i = 0; i < 1; i++)
	{
		moverVibora(listaViboras[0], maxX, maxY);
		dibujarVibora(listaViboras[0]);
		escucharInput_jugador();
	} 
		
	refresh(); //actualiza ncurses
		
	usleep(juegoVelocidad); //retrasa el programa
}

int jugarJuego(int nivelSeleccionado, int velocidadSeleccionada)
{
	inicializarJuego();
	while (juegoActivo)
	{
		actualizarJuego();
	}
	return juegoActivo;
} 
