#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define TECLA_ESC 27
#define TECLA_Q 113

#define TECLA_ARRIBA 259 //sin utilizar
#define TECLA_ABAJO 258 //sin utilizar
#define TECLA_IZQUIERDA 260
#define TECLA_DERECHA 261

#define TECLA_W 119 //sin utilizar
#define TECLA_A 97
#define TECLA_S 115 //sin utilizar
#define TECLA_D 100

enum Direccion { arriba, abajo, izquierda, derecha };

typedef struct Punto
{
	int x;
	int y;
	char* dibujo;
	struct Punto* sig;
} Punto;

// crea punto en la vibora. se utiliza para actualizar la nueva
// posicion de la cabeza
Punto *crearPunto(int x, int y, Punto *sig)
{
    Punto *nuevoPunto = malloc(sizeof *nuevoPunto);
    if (nuevoPunto == NULL) { return NULL; }
    nuevoPunto->x = x;
    nuevoPunto->y = y;
    nuevoPunto->sig = sig;
    return nuevoPunto;
}

// elimina el ultimo punto de la vibora, se utiliza para eliminar
// los puntos rezagados de la vibora cuando se mueve
void eliminarUltimoPunto(Punto *cabeza)
{
    if (cabeza == NULL) { return; }
    Punto **actual = &cabeza;
    while ((*actual)->sig != NULL)
    {
		actual = &(*actual)->sig;
	}
    *actual = NULL;
    free(*actual);
}

Punto *cabezaVibora = NULL; // la cabeza de la lista ligada vibora

enum Direccion direccionActual = arriba;

int juegoActivo = 1; //1 para continuar jugando y 0 para salir
int velocidad = 50000; //velocidad en microsegundos de refrescamiento
int longitudVibora = 1; //longitud de vibora inicial, 1 por la cabeza
int longitudViboraMaxima = 7; //longitud maxima de la vibora

int maxX = 0; //ncurses maximo tamano X de terminal
int maxY = 0; //ncurses maximo tamano Y de terminal

// redirecciona la vibora hacia la izquierda en direccion relativa
enum Direccion redireccionarIzquierda()
{
	enum Direccion nuevaDireccion;
	
	if (direccionActual == arriba) { nuevaDireccion = izquierda; }
	else if (direccionActual == abajo) { nuevaDireccion = derecha; }
	else if (direccionActual == izquierda) { nuevaDireccion = abajo; }
	else if (direccionActual == derecha) { nuevaDireccion = arriba; }
	
	return nuevaDireccion;
}

// redirecciona la vibora hacia la derecha en direccion relativa
enum Direccion redireccionarDerecha()
{
	enum Direccion nuevaDireccion;
	
	if (direccionActual == arriba) { nuevaDireccion = derecha; }
	else if (direccionActual == abajo) { nuevaDireccion = izquierda; }
	else if (direccionActual == izquierda) { nuevaDireccion = arriba; }
	else if (direccionActual == derecha) { nuevaDireccion = abajo; }
	
	return nuevaDireccion;
}

// dibuja la vibora en pantalla
void dibujarVibora()
{
	// imprimir la vibora y su cola
	mvprintw(cabezaVibora->y,cabezaVibora->x, cabezaVibora->dibujo);
	
	Punto *cola;
	cola = cabezaVibora->sig;
	
	while (cola->sig != NULL)
	{
		mvprintw(cola->y,cola->x, "o");
		cola = cola->sig;
	}
	mvprintw(cola->y,cola->x, ".");
}

// mueve la vibora de posicion
void moverVibora()
{
	// mover la vibora respecto a su direccion
	if (direccionActual == arriba) 
	{
		cabezaVibora = crearPunto(cabezaVibora->x,
								  cabezaVibora->y-1, 
								  cabezaVibora);
		longitudVibora++;
		cabezaVibora->dibujo = "V";
	}
	else if (direccionActual == abajo) 
	{
		cabezaVibora = crearPunto(cabezaVibora->x,
								  cabezaVibora->y+1, 
								  cabezaVibora);
		longitudVibora++;
		cabezaVibora->dibujo = "A";
	}
	else if (direccionActual == izquierda) 
	{
		cabezaVibora = crearPunto(cabezaVibora->x-1,
								  cabezaVibora->y, 
								  cabezaVibora);
		longitudVibora++;
		cabezaVibora->dibujo = ">";
	}
	else if (direccionActual == derecha) 
	{
		cabezaVibora = crearPunto(cabezaVibora->x+1,
								  cabezaVibora->y, 
								  cabezaVibora);
		longitudVibora++;
		cabezaVibora->dibujo = "<";
	}
	
	// eliminar el ultimo punto de la cola por longitud maxima
	if (longitudVibora-1 > longitudViboraMaxima)
	{
		eliminarUltimoPunto(cabezaVibora);
		longitudVibora--;
	}
	
	// mover la vibora al otro extremo en caso de salir del margen
	if (cabezaVibora->x > maxX) { cabezaVibora->x = 0; }
	if (cabezaVibora->x < 0) { cabezaVibora->x = maxX; }
	
	if (cabezaVibora->y > maxY) { cabezaVibora->y = 0; }
	if (cabezaVibora->y < 0) { cabezaVibora->y = maxY; }
}

// inicializa el programa y ncurses
void inicializar()
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
}

// sale del juego
void salir()
{
	juegoActivo = 0;
}

// lee el input del usuario y lo convierte a comandos
void obtenerInput()
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
}

// actualiza la pantalla ncurses
void actualizarPantalla()
{
		clear(); //limpia la pantalla
		
		moverVibora();
		dibujarVibora();
		
		refresh(); //actualiza ncurses
		
		usleep(velocidad); //retrasa el programa
}

int main()
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
}
