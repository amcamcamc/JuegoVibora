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
int juegoActivo = 0; //1 para continuar jugando y 0 para salir
int juegoVelocidad = 100000; //tiempo en microsegundos entre actualizaciones de pantalla
int nivel = 0; //el nivel seleccionado
int finDelJuego = -1; //se utiliza para saber el ganador del juego. 1 para el usuario, 2 para la computadora, 0 es empate

//Datos ncurses
int maxX = 0;
int maxY = 0;

//Escucha el input del jugador y lo traduce a comandos para el juego
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

//Aparece una manzana en el mapa con las posiciones indicadas
void aparecerManzana(Elemento *manzanaNueva, int posX, int posY)
{
	manzanaNueva->x = posX;
    manzanaNueva->y = posY;
    manzanaNueva->dibujo = "a";
}

//Mueve la manzana actual de lugar y la cambia a una posicion al azar
//Util para cuando una vibora se la come
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

//Dibujar la manzana en el escenario
void dibujarManzana(Elemento *manzanaObj)
{
	attron(COLOR_PAIR(7));
	mvprintw(manzanaObj->y, manzanaObj->x, manzanaObj->dibujo);
}

//Come la manzana pasada especificando cual vibora se la comio
void comerManzana(Elemento *manzanaObj, Vibora *viboraComedora)
{
	viboraComedora->longitudMax = viboraComedora->longitudMax+1;
	viboraComedora->puntuacion = viboraComedora->puntuacion+1;
	viboraComedora->cabeza->dibujo = "O";
	
	moverManzana(manzanaObj);
}

//Detecta colisiones en el juego y decide si continuar o salir
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

//Actualiza la posicion de las viboras y las dibuja
void actualizarViboras()
{
	for (int i = 0; i < 2; i++)
	{
		if (juegoActivo)
		{
			moverVibora(listaViboras[i], maxX, maxY);
		}
		attron(COLOR_PAIR(listaViboras[i]->color));
		dibujarVibora(listaViboras[i]);
	}
}

//Muestra informacion del juego que solo es visible cuando la terminal esta en pantalla completa
void mostrarInfoJuego()
{
	mvprintw(26,1,"[Jugador] %d puntos",viboraJugador->puntuacion);
	mvprintw(27,1,"[Computadora] %d puntos",viboraComputadora->puntuacion);
	//vprintw(28,1,"Manzana: (x:%d,y:%d)",manzana->x,manzana->y);
}

//Los comandos a correr cuando se carga el juego
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
	
	viboraJugador = crearVibora(1, (maxX/2)+5, maxY/2, arriba);
	viboraComputadora = crearVibora(2, (maxX/2)-5, maxY/2, abajo);
	
	if (viboraJugador != NULL)
	{
		listaViboras[0] = viboraJugador;
		listaViboras[1] = viboraComputadora;
	}
	
	manzana = crearElemento(-1,-1, NULL);
	moverManzana(manzana);
	
	finDelJuego = -1;
}

//Funcion que manda a llamar a la vibora de la computadora para que se mueva
void controlarRival()
{
	analizarJuego(viboraComputadora, viboraJugador, manzana, &escenarioActual);
}

//Actualiza el juego por cada cuadro
void actualizarJuego()
{
	//getmaxyx(stdscr, maxY, maxX);
	
	clear(); //limpia la pantalla
	
	
	//Es importante que las funciones esten en este orden especifico
	
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

//Termina el juego indicando el ganador
void terminarJuego(int ganador)
{
	finDelJuego = ganador;
	juegoActivo = 0;
}

//Manda a inicializar el juego y lo actualiza hasta que acabe o el usuario salga
int jugarJuego()
{
	inicializarJuego();
	while (juegoActivo)
	{
		actualizarJuego();
	}
	return juegoActivo;
} 
