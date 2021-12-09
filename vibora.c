#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "vibora.h"

Elemento *crearElemento(int x, int y, Elemento *sig)
{
    Elemento *elementoNuevo = malloc(sizeof *elementoNuevo);
    if (elementoNuevo == NULL) { return NULL; }
    elementoNuevo->x = x;
    elementoNuevo->y = y;
    elementoNuevo->sig = sig;
    return elementoNuevo;
}

void eliminarUltimoElemento(Elemento *cabeza)
{
    if (cabeza == NULL) { return; }
    Elemento **actual = &cabeza;
    while ((*actual)->sig != NULL)
    {
		actual = &(*actual)->sig;
	}
    *actual = NULL;
    free(*actual);
}

Vibora *crearVibora(char nombre[32], int color, int posX, int posY)
{
	Vibora *viboraNueva = malloc(sizeof *viboraNueva);
	if (viboraNueva == NULL) { return NULL; }
	//viboraNueva->nombre = nombre;
	viboraNueva->color = color;
	viboraNueva->longitud = 1;
	viboraNueva->longitudMax = 1;
	viboraNueva->puntuacion = 0;
	viboraNueva->direccion = derecha;
	viboraNueva->cabeza = crearElemento(posX, posY, NULL);
	if (viboraNueva->cabeza == NULL) { return NULL; }
	return viboraNueva;
}

// redirecciona la vibora hacia la izquierda en direccion relativa
enum Direccion redireccionarIzquierda(enum Direccion direccionActual)
{
	enum Direccion nuevaDireccion;
	
	if (direccionActual == arriba) { nuevaDireccion = izquierda; }
	else if (direccionActual == abajo) { nuevaDireccion = derecha; }
	else if (direccionActual == izquierda) { nuevaDireccion = abajo; }
	else if (direccionActual == derecha) { nuevaDireccion = arriba; }
	
	return nuevaDireccion;
}

// redirecciona la vibora hacia la derecha en direccion relativa
enum Direccion redireccionarDerecha(enum Direccion direccionActual)
{
	enum Direccion nuevaDireccion;
	
	if (direccionActual == arriba) { nuevaDireccion = derecha; }
	else if (direccionActual == abajo) { nuevaDireccion = izquierda; }
	else if (direccionActual == izquierda) { nuevaDireccion = arriba; }
	else if (direccionActual == derecha) { nuevaDireccion = abajo; }
	
	return nuevaDireccion;
}

// dibuja la vibora en pantalla
void dibujarVibora(Vibora *vibora)
{
	// imprimir la vibora y su cola
	mvprintw(vibora->cabeza->y,vibora->cabeza->x, vibora->cabeza->dibujo);
	
	Elemento *cola;
	cola = vibora->cabeza->sig;
	
	while (cola->sig != NULL)
	{
		mvprintw(cola->y,cola->x, "o");
		cola = cola->sig;
	}
	mvprintw(cola->y,cola->x, ".");
}

// mueve la vibora de posicion
void moverVibora(Vibora *vibora, int limiteX, int limiteY)
{
	// mover la vibora respecto a su direccion
	if (vibora->direccion == arriba) 
	{
		vibora->cabeza = crearElemento(vibora->cabeza->x,
									   vibora->cabeza->y-1, 
								       vibora->cabeza);
		vibora->longitud = vibora->longitud+1;
		vibora->cabeza->dibujo = "V";
	}
	else if (vibora->direccion == abajo) 
	{
		vibora->cabeza = crearElemento(vibora->cabeza->x,
									   vibora->cabeza->y+1, 
								       vibora->cabeza);
		vibora->longitud = vibora->longitud+1;
		vibora->cabeza->dibujo = "A";
	}
	else if (vibora->direccion == izquierda) 
	{
		vibora->cabeza = crearElemento(vibora->cabeza->x-1,
									   vibora->cabeza->y, 
									   vibora->cabeza);
		vibora->longitud = vibora->longitud+1;
		vibora->cabeza->dibujo = ">";
	}
	else if (vibora->direccion == derecha) 
	{
		vibora->cabeza = crearElemento(vibora->cabeza->x+1,
									   vibora->cabeza->y, 
									   vibora->cabeza);
		vibora->longitud = vibora->longitud+1;
		vibora->cabeza->dibujo = "<";
	}
	
	// eliminar el ultimo punto de la cola por longitud maxima
	if (vibora->longitud-1 > vibora->longitudMax)
	{
		eliminarUltimoElemento(vibora->cabeza);
		vibora->longitud = vibora->longitud - 1;
	}
	
	// mover la vibora al otro extremo en caso de salir del margen
	if (vibora->cabeza->x > limiteX) { vibora->cabeza->x = 0; }
	if (vibora->cabeza->x < 0) { vibora->cabeza->x = limiteX; }
	
	if (vibora->cabeza->y > limiteY) { vibora->cabeza->y = 0; }
	if (vibora->cabeza->y < 0) { vibora->cabeza->y = limiteY; }
}

// inicializa el programa y ncurses
/*void inicializar()
{
	initscr(); //inicializa la ventana
	noecho(); //no imprimir teclas presionadas
	keypad(stdscr, TRUE); //abilita el presionar teclas
	curs_set(FALSE); //no mostrar cursor
	cbreak();
	
	getmaxyx(stdscr, maxY, maxX); //obtener tamanos maximos terminal
	
	cabezaVibora = crearPunto(maxX/2, maxY/2, NULL);
	
	direccionActual = redireccionarDerecha();
	timeout(0);
}*/

// sale del juego
/*void salir()
{
	juegoActivo = 0;
}*/

// lee el input del usuario y lo convierte a comandos
/*void obtenerInput()
{
	int tecla = getch();
	//printw("%d\n",tecla);
	refresh();
	switch(tecla)
	{
		case TECLA_A:
			direccionActual = redireccionarIzquierda();
			break;
		case TECLA_D:
			direccionActual = redireccionarDerecha();
			break;
		case TECLA_IZQUIERDA:
			direccionActual = redireccionarIzquierda();
			break;
		case TECLA_DERECHA:
			direccionActual = redireccionarDerecha();
			break;		
		case TECLA_ESC:
			salir();
			break;
		case TECLA_Q:
			salir();
			break;
	}
}*/

// actualiza la pantalla ncurses
/*void actualizarPantalla()
{
		clear(); //limpia la pantalla
		
		moverVibora();
		dibujarVibora();
		
		refresh(); //actualiza ncurses
		
		usleep(velocidad); //retrasa el programa
}*/

/*int main()
{
	inicializar(); 
	
	while (juegoActivo)
	{
		getmaxyx(stdscr, maxY, maxX); //por si la consola cambia tamano
		obtenerInput();
		actualizarPantalla();
	}
	
	endwin(); //regresa la terminal a su estado normal
	nocbreak();
	
	return 0;
}*/
