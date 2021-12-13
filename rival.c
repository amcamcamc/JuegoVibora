#include "rival.h"
#include "vibora.h"
#include "escenario.h"

int obstaculoEncontrado = 0;

enum Direccion calcularRedireccionamiento(enum Direccion actual, enum Direccion deseada)
{
	if (actual == arriba)
	{
		if (deseada == arriba) return arriba;
		if (deseada == derecha) return redireccionarDerecha(actual);
		if (deseada == izquierda) return redireccionarIzquierda(actual);
		if (deseada == abajo) return redireccionarIzquierda(actual);
	}
	else if (actual == izquierda)
	{
		if (deseada == arriba) return redireccionarDerecha(actual);
		if (deseada == derecha) return redireccionarIzquierda(actual);
		if (deseada == izquierda) return izquierda;
		if (deseada == abajo) return redireccionarIzquierda(actual);
	}
	else if (actual == abajo)
	{
		if (deseada == arriba) return redireccionarIzquierda(actual);
		if (deseada == derecha) return redireccionarIzquierda(actual);
		if (deseada == izquierda) return redireccionarDerecha(actual);
		if (deseada == abajo) return abajo;
	}
	else if (actual == derecha)
	{
		if (deseada == arriba) return redireccionarIzquierda(actual);
		if (deseada == derecha) return derecha;
		if (deseada == izquierda) return redireccionarIzquierda(actual);
		if (deseada == abajo) return redireccionarDerecha(actual);
	}
	return actual;
}

void esquivarObstaculos_Escenario(Escenario *escenario, Vibora *computadora)
{
	obstaculoEncontrado = 0;
	//Si la computadora va a chocar con el escenario cuando va hacia arriba
	if (detectarColision_Escenario(computadora->cabeza->x, computadora->cabeza->y-1, escenario))
	{
		obstaculoEncontrado = 1;
		//Detectar si los lados se encuentra libre
			
		if (!detectarColision_Escenario(computadora->cabeza->x-1, computadora->cabeza->y, escenario))
		{
			//La izquierda esta libre
			computadora->direccion = calcularRedireccionamiento(arriba, izquierda);
		}
		if (!detectarColision_Escenario(computadora->cabeza->x+1, computadora->cabeza->y, escenario))
		{
			//La derecha esta libre
			computadora->direccion = calcularRedireccionamiento(arriba, derecha);
		}
	}
	//Si la computadora va a chocar con el escenario cuando va hacia la izquierda
	else if (detectarColision_Escenario(computadora->cabeza->x-1, computadora->cabeza->y, escenario))
	{
		obstaculoEncontrado = 1;
		//Detectar si los lados se encuentra libre
			
		if (!detectarColision_Escenario(computadora->cabeza->x, computadora->cabeza->y+1, escenario))
		{
			//La izquierda esta libre
			computadora->direccion = calcularRedireccionamiento(izquierda, abajo);
		}
		if (!detectarColision_Escenario(computadora->cabeza->x, computadora->cabeza->y-1, escenario))
		{
			//La derecha esta libre
			computadora->direccion = calcularRedireccionamiento(izquierda, arriba);
		}
	}
	//Si la computadora va a chocar con el escenario cuando va hacia abajo
	else if (detectarColision_Escenario(computadora->cabeza->x, computadora->cabeza->y+1, escenario))
	{
		obstaculoEncontrado = 1;
		//Detectar si los lados se encuentra libre
		
		if (!detectarColision_Escenario(computadora->cabeza->x-1, computadora->cabeza->y, escenario))
		{
			//La izquierda esta libre
			computadora->direccion = calcularRedireccionamiento(abajo, izquierda);
		}
		if (!detectarColision_Escenario(computadora->cabeza->x+1, computadora->cabeza->y, escenario))
		{
			//La derecha esta libre
			computadora->direccion = calcularRedireccionamiento(abajo, derecha);
		}
	}
	//Si la computadora va a chocar con el escenario cuando va hacia la derecha
	else if (detectarColision_Escenario(computadora->cabeza->x+1, computadora->cabeza->y, escenario))
	{
		obstaculoEncontrado = 1;
		//Detectar si los lados se encuentra libre
		
		if (!detectarColision_Escenario(computadora->cabeza->x, computadora->cabeza->y-1, escenario))
		{
			//La izquierda esta libre
			computadora->direccion = calcularRedireccionamiento(derecha, arriba);
		}
		if (!detectarColision_Escenario(computadora->cabeza->x, computadora->cabeza->y+1, escenario))
		{
			//La derecha esta libre
			computadora->direccion = calcularRedireccionamiento(derecha, abajo);
		}
	}
}  

void alinearConManzana(Elemento *manzanaObj, Vibora *computadora)
{
	if (obstaculoEncontrado) return;
	int alineadoX = 0;
	int alineadoY = 0;
	
	//Si esta alineada la vibora
	if (manzanaObj->x == computadora->cabeza->x)
	{ alineadoX = 1; }		
	if (manzanaObj->y == computadora->cabeza->y)
	{ alineadoY = 1; }
	
	if (!alineadoX)
	{	
		//Si la manzana se encuentra a la derecha de la posicion
		//actual de la vibora de la computadora
		if (manzanaObj->x > computadora->cabeza->x)
		{
			//Entonces nos tenemos que alinear con el eje horizontal
			//Y dirigirnos a la derecha
			if (computadora->direccion != derecha)
			{ computadora->direccion = calcularRedireccionamiento(computadora->direccion, derecha); }
		}
		
		//Si la manzana se encuentra a la izquierda de la posicion
		//actual de la vibora de la computadora
		if (manzanaObj->x < computadora->cabeza->x)
		{
			//Entonces nos tenemos que alinear con el eje horizontal
			//Y dirigirnos a la derecha
			if (computadora->direccion != izquierda)
			{ computadora->direccion = calcularRedireccionamiento(computadora->direccion, izquierda); }
		}
		
		return;
	}
	
	if (!alineadoY)
	{
		//Si la manzana se encuentra hacia arriba de la posicion
		//actual de la vibora de la computadora
		if (manzanaObj->y < computadora->cabeza->y)
		{
			//Entonces nos tenemos que alinear con el eje vertical
			//Y dirigirnos hacia arriba
			if (computadora->direccion != arriba)
			{ computadora->direccion = calcularRedireccionamiento(computadora->direccion, arriba); }
		}
		
		//Si la manzana se encuentra hacia abajo de la posicion
		//actual de la vibora de la computadora
		if (manzanaObj->y > computadora->cabeza->y)
		{
			//Entonces nos tenemos que alinear con el eje vertical
			//Y dirigirnos hacia abajo
			if (computadora->direccion != abajo)
			{ computadora->direccion = calcularRedireccionamiento(computadora->direccion, abajo); }
		}
		
		return;
	}
}

void analizarJuego(Vibora *computadora, Vibora *jugador, Elemento *manzanaObj, Escenario *escenario)
{
	esquivarObstaculos_Escenario(escenario, computadora);
	alinearConManzana(manzanaObj, computadora);
}
