#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "controles.h"
#include "vibora.h"
#include "rival.h"
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
int finDelJuego = -1;

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

void moverManzana(Elemento *manzanaActual)
{
	//Localizar coordenadas que no esten bloqueadas por viboras o 
	//por los obstaculos del escenario para colocar ahi la manzana
	int puntoLocalizado = 0;
	int randX;
	int randY;
	
	while (puntoLocalizado == 0)
	{
		randX = rand() % maxX;
		randY = rand() % maxY;
				
		manzanaActual->x = randX;
		manzanaActual->y = randY;
		
		if (detectarColision_Escenario(manzanaActual->x,manzanaActual->y, &escenarioActual) == 0)
		{
			if (detectarColision_Vibora(manzanaActual, viboraJugador->cabeza,0) == 0 && 
				detectarColision_Vibora(manzanaActual, viboraComputadora->cabeza,0) == 0
			   )
			{
				aparecerManzana(manzanaActual, randX, randY);
				puntoLocalizado = 1;
				return;
			}
		}
	}
}

void dibujarManzana(Elemento *manzanaObj)
{
	attron(COLOR_PAIR(7));
	mvprintw(manzanaObj->y, manzanaObj->x, manzanaObj->dibujo);
}

void comerManzana(Elemento *manzanaObj, Vibora *viboraComedora)
{
	viboraComedora->longitudMax = viboraComedora->longitudMax+1;
	viboraComedora->puntuacion = viboraComedora->puntuacion+1;
	viboraComedora->cabeza->dibujo = "O";
	
	moverManzana(manzanaObj);
}

void detectarColisiones()
{
	int colisionJugador_Manzana = detectarColision_Manzana(viboraJugador->cabeza, manzana);
	int colisionComputadora_Manzana = detectarColision_Manzana(viboraComputadora->cabeza, manzana);
	
	int colisionJugador_Vibora = detectarColision_Vibora(viboraJugador->cabeza, viboraComputadora->cabeza,0);
	int colisionComputadora_Vibora = detectarColision_Vibora(viboraComputadora->cabeza, viboraJugador->cabeza,0);
	
	int colisionJugador_Jugador = detectarColision_Vibora(viboraJugador->cabeza, viboraJugador->cabeza,1);
	int colisionComputadora_Computadora = detectarColision_Vibora(viboraComputadora->cabeza, viboraComputadora->cabeza,1);
	
	int colisionJugador_Escenario = detectarColision_Escenario(viboraJugador->cabeza->x, viboraJugador->cabeza->y, &escenarioActual);
	int colisionComputadora_Escenario = detectarColision_Escenario(viboraComputadora->cabeza->x, viboraComputadora->cabeza->y, &escenarioActual);
	
	//--------------------------------------------------------------
	//Colision con la manzana
	
	if (colisionJugador_Manzana) { comerManzana(manzana, viboraJugador); }
	if (colisionComputadora_Manzana) { comerManzana(manzana, viboraComputadora); }
	
	//--------------------------------------------------------------
	//Colision con si misma
	
	//El jugador ha chocado con su misma cola
	if (colisionJugador_Jugador == 1)
	{
		viboraJugador->viva = 0; terminarJuego(2); //Gana la computadora
		return;
	}

	//La computadora ha chocado con su misma cola
	if (colisionComputadora_Computadora == 1)
	{
		viboraComputadora->viva = 0; terminarJuego(1); //Gana el jugador
		return;
	}
	
	//--------------------------------------------------------------
	//Colision con otras viboras
	
	//El jugador ha chocado con la computadora
	if (colisionJugador_Vibora == 1)
	{
		viboraJugador->viva = 0; terminarJuego(2); //Gana la computadora
		return;
	}
	
	//La computadora ha chocado con el jugador
	if (colisionComputadora_Vibora == 1)
	{
		viboraComputadora->viva = 0; terminarJuego(1); //Gana el jugador
		return;
	}

	//Ambas viboras han chocado entre si
	if (colisionJugador_Vibora == 2 && colisionComputadora_Vibora == 2)
	{
		viboraJugador->viva = 0; viboraComputadora->viva = 0;
		terminarJuego(0); //Empate por colision mutua
		return;
	}
	
	//--------------------------------------------------------------
	//Colision con el escenario
	
	//Si el jugador ha chocado con el escenario
	if (colisionJugador_Escenario)
	{
		//Si la computadora tambien ha chocado con el escenario
		if (colisionComputadora_Vibora)
		{
			viboraJugador->viva = 0;
			viboraComputadora->viva = 0;
			terminarJuego(0); //Empate por colision mutua
			return;
		}
		viboraJugador->viva = 0;
		terminarJuego(2); //Gana la computadora
	}
	
	//Si la computadora ha chocado con el escenario
	if (colisionComputadora_Escenario)
	{
		//Si el jugador tambien ha chocado con el escenario
		if (colisionJugador_Vibora)
		{
			viboraJugador->viva = 0;
			viboraComputadora->viva = 0;
			terminarJuego(0); //Empate por colision mutua
			return;
		}
		viboraComputadora->viva = 0;
		terminarJuego(1); //Gana el jugador
		return;
	}
}

void actualizarViboras()
{
	for (int i = 0; i < 2; i++)
	{
		if (juegoActivo)
		{
			moverVibora(listaViboras[i], maxX, maxY);
		}
		attron(COLOR_PAIR(i+1));
		dibujarVibora(listaViboras[i]);
	}
}

void mostrarInfoJuego()
{
	mvprintw(26,1,"[Jugador] cabeza: (x:%d,y:%d) | long: %d | puntos: %d | dir: %d",
			 viboraJugador->cabeza->x,
			 viboraJugador->cabeza->y,
			 viboraJugador->longitud,
			 viboraJugador->puntuacion,
			 viboraJugador->direccion
			);
	mvprintw(27,1,"[Computadora] cabeza: (x:%d,y:%d) | long: %d | puntos: %d | dir: %d",
			 viboraComputadora->cabeza->x,
			 viboraComputadora->cabeza->y,
			 viboraComputadora->longitud,
			 viboraComputadora->puntuacion,
			 viboraComputadora->direccion
			);
	mvprintw(28,1,"Manzana: (x:%d,y:%d)",manzana->x,manzana->y);
	if (viboraJugador->longitud >= 7)
	{
		mvprintw(29,1,"ColisionJJ: %d", detectarColision_Vibora(viboraJugador->cabeza, viboraJugador->cabeza,1));
	}
}

void inicializarJuego()
{
	char* nivelSeleccionado;
	
	if (nivel == 1) { nivelSeleccionado = "nivel1.txt"; }
	if (nivel == 2) { nivelSeleccionado = "nivel2.txt"; }
	if (nivel == 3) { nivelSeleccionado = "nivel3.txt"; }
	
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
	moverManzana(manzana);
	
	finDelJuego = -1;
}

void controlarRival()
{
	analizarJuego(viboraComputadora, viboraJugador, manzana, &escenarioActual);
}

void actualizarJuego()
{
	//getmaxyx(stdscr, maxY, maxX);
	
	clear(); //limpia la pantalla
	
	imprimirEscenario(&escenarioActual, maxX, maxY);
	dibujarManzana(manzana);
	detectarColisiones();
	actualizarViboras();
	controlarRival();
	escucharInput_jugador();
	
	mostrarInfoJuego();
		
	refresh(); //actualiza ncurses
		
	usleep(juegoVelocidad); //retrasa el programa
}

void terminarJuego(int ganador)
{
	finDelJuego = ganador;
	juegoActivo = 0;
}

int jugarJuego()
{
	inicializarJuego();
	while (juegoActivo)
	{
		actualizarJuego();
	}
	return juegoActivo;
} 
