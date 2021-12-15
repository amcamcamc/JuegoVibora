#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "vibora.h"

//Crea un elemento. en contexto de las viboras, se utiliza para 
//mover la cabeza de la vibora
Elemento *crearElemento(int x, int y, Elemento *sig)
{
    Elemento *elementoNuevo = malloc(sizeof *elementoNuevo);
    if (elementoNuevo == NULL) { return NULL; }
    elementoNuevo->x = x;
    elementoNuevo->y = y;
    elementoNuevo->sig = sig;
    return elementoNuevo;
}

//Elimina el ultimo punto de la vibora, se utiliza para eliminar
//los puntos rezagados de la vibora cuando se mueve
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

//Inicializa un objeto vibora
Vibora *crearVibora(int color, int posX, int posY, enum Direccion direccionInicial)
{
	Vibora *viboraNueva = malloc(sizeof *viboraNueva);
	if (viboraNueva == NULL) { return NULL; }
	viboraNueva->color = color;
	viboraNueva->longitud = 1;
	viboraNueva->longitudMax = 3;
	viboraNueva->puntuacion = 0;
	viboraNueva->viva = 1;
	viboraNueva->direccion = direccionInicial;
	viboraNueva->cabeza = crearElemento(posX, posY, NULL);
	if (viboraNueva->cabeza == NULL) { return NULL; }
	return viboraNueva;
}

//Redirecciona la vibora hacia la izquierda en direccion relativa
enum Direccion redireccionarIzquierda(enum Direccion direccionActual)
{
	enum Direccion nuevaDireccion;
	
	if (direccionActual == arriba) { nuevaDireccion = izquierda; }
	else if (direccionActual == abajo) { nuevaDireccion = derecha; }
	else if (direccionActual == izquierda) { nuevaDireccion = abajo; }
	else if (direccionActual == derecha) { nuevaDireccion = arriba; }
	
	return nuevaDireccion;
}

//Redirecciona la vibora hacia la derecha en direccion relativa
enum Direccion redireccionarDerecha(enum Direccion direccionActual)
{
	enum Direccion nuevaDireccion;
	
	if (direccionActual == arriba) { nuevaDireccion = derecha; }
	else if (direccionActual == abajo) { nuevaDireccion = izquierda; }
	else if (direccionActual == izquierda) { nuevaDireccion = arriba; }
	else if (direccionActual == derecha) { nuevaDireccion = abajo; }
	
	return nuevaDireccion;
}

//Dibuja la vibora en pantalla
void dibujarVibora(Vibora *vibora)
{
	//Imprimir la vibora y su cola
	mvprintw(vibora->cabeza->y,vibora->cabeza->x, vibora->cabeza->dibujo);
	
	Elemento *cola;
	cola = vibora->cabeza->sig;
	
	while (cola->sig != NULL)
	{
		if (strcmp(cola->dibujo, "O") == 0) { mvprintw(cola->y,cola->x, "@"); }
		else { mvprintw(cola->y,cola->x, "o"); }
		cola = cola->sig;
	}
	mvprintw(cola->y,cola->x, ".");
}

//Mueve la vibora de posicion
void moverVibora(Vibora *vibora, int limiteX, int limiteY)
{
	//Mover la vibora respecto a su direccion
	if (vibora->direccion == arriba) 
	{
		vibora->cabeza = crearElemento(vibora->cabeza->x,
									   vibora->cabeza->y-1, 
								       vibora->cabeza);
		vibora->longitud = vibora->longitud+1;
		vibora->cabeza->dibujo = "v";
	}
	else if (vibora->direccion == abajo) 
	{
		vibora->cabeza = crearElemento(vibora->cabeza->x,
									   vibora->cabeza->y+1, 
								       vibora->cabeza);
		vibora->longitud = vibora->longitud+1;
		vibora->cabeza->dibujo = "^";
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
	
	//Eliminar el ultimo punto de la cola por longitud maxima
	if (vibora->longitud > vibora->longitudMax)
	{
		eliminarUltimoElemento(vibora->cabeza);
		vibora->longitud = vibora->longitud - 1;
	}
	
	//Mover la vibora al otro extremo en caso de salir del margen
	if (vibora->cabeza->x >= limiteX) { vibora->cabeza->x = 0; }
	if (vibora->cabeza->x < 0) { vibora->cabeza->x = limiteX-1; }
	
	if (vibora->cabeza->y >= limiteY) { vibora->cabeza->y = 0; }
	if (vibora->cabeza->y < 0) { vibora->cabeza->y = limiteY-1; }
}

//Detecta si la cabeza de la vibora chocaria con algun elemento de la vibora obstaculo
int detectarColision_Vibora(Elemento *cabezaDetectora, Elemento *cabezaObstaculo, int misma)
{
	//Detectar colision mutua solo si no es la misma vibora
	if (misma == 0)
	{
		if (cabezaDetectora->x == cabezaObstaculo->x && cabezaDetectora->y == cabezaObstaculo->y)
		{ return 2; }
	}

    Elemento **actual = &cabezaObstaculo;
    
    //Si estamos comparando con la misma vibora, no queremos contar la cabeza
    if (misma && (*actual)->sig != NULL) { actual = &(*actual)->sig; }
    
    while ((*actual)->sig != NULL)
    {
		if ((*actual)->x == cabezaDetectora->x && (*actual)->y == cabezaDetectora->y)
		{
			return 1; //La cabeza detectora choca con la cabeza obstaculo
		}
		actual = &(*actual)->sig;
	}
	return 0; //Sin colision
}

// Detecta si la cabeza de la vibora se come una manzana
int detectarColision_Manzana(Elemento *cabezaDetectora, Elemento *manzanaObj)
{
	if (cabezaDetectora->x == manzanaObj->x && cabezaDetectora->y == manzanaObj->y)
	{ return 1; }
	return 0;
}
