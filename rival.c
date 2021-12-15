#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ncurses.h>

#include "rival.h"
#include "vibora.h"
#include "escenario.h"

Grafo grafoEscenario;
ListaEspera listaEspera;
int grafoInicializado = 0;

void crearListaEspera(ListaEspera *lista, int numVerticesTotal)
{
	lista->tamanoMax = numVerticesTotal;
	lista->alFrente = 0;
	lista->tamano = 0;
	
	lista->alFinal = numVerticesTotal - 1;
	lista->vertices = (Vertice*)calloc(numVerticesTotal, sizeof(Vertice));
}

int listaEsperaVacia(ListaEspera *lista)
{
	return (lista->tamano == 0);
}

void agregarListaEspera(ListaEspera *lista, Vertice vertice)
{
	lista->alFinal = (lista->alFinal + 1) % lista->tamanoMax;
	lista->vertices[lista->alFinal] = vertice;
	lista->tamano = lista->tamano + 1;
}

Vertice quitarListaEspera(ListaEspera *lista)
{
	Vertice vertice = lista->vertices[lista->alFrente];
	lista->alFrente = (lista->alFrente + 1) % lista->tamanoMax;
	lista->tamano = lista->tamano - 1;
	return vertice;
}

//Transforma coordenadas en pantalla a indice en arreglo 1D
int XY_a_Idx(int x, int y)
{
	return (x + (y * 80));
}

void initGrafo(Grafo *grafo, int numVerticesTotal)
{	
	grafo->numVerticesMax = numVerticesTotal;
	grafo->numVertices = 0;
	grafo->numAristas = 0;
	
	//Obtenemos memoria para la matriz de adyacencia.
	grafo->matrizAdy = (int**)malloc(numVerticesTotal * sizeof(int*));
	for (int i = 0; i < numVerticesTotal; i++)
	{
		grafo->matrizAdy[i] = (int*)calloc(numVerticesTotal, sizeof(int));
	}
	
	//Obtenemos memoria para la lista de vertices.
	grafo->vertices = (Vertice*)calloc(numVerticesTotal, sizeof(Vertice));
}

int encontrarVertice(Grafo *grafo, int idxBuscado)
{
	int idxEncontrado = grafo->vertices[idxBuscado].idx;
	int idxAsignado = grafo->vertices[idxBuscado].asignado;
	if (idxAsignado != 0)
	{
		return idxEncontrado;
	}
	return -1;
}

int agregarVertice(Grafo *grafo, int idxNuevo)
{	
	//Validemos que haya espacio para agregar un vértice
	if (grafo->numVertices == grafo->numVerticesMax) { return -1; }
		
	//Validar que el vertice no exista aun
	if (encontrarVertice(grafo, idxNuevo) != -1) { return 0; }
		
	//Agregar el vertice
	grafo->vertices[grafo->numVertices].idx = grafo->numVertices;
	grafo->vertices[grafo->numVertices].asignado = 1;
	grafo->numVertices = grafo->numVertices+1;
	
	return 0;
}

int encontrarArista(Grafo *grafo, int orig, int dest)
{	
	return grafo->matrizAdy[orig][dest];
}

void agregarArista(Grafo *grafo, int orig, int dest)
{
	if (encontrarArista(grafo, orig, dest) == 1) { return; }
	if (encontrarArista(grafo, dest, orig) == 1) { return; }
	grafo->matrizAdy[orig][dest] = 1;
	grafo->numAristas = grafo->numAristas+1;
}

void eliminarArista(Grafo *grafo, int orig, int dest)
{
	if (encontrarArista(grafo, orig, dest) != 1 &&
		encontrarArista(grafo, dest, orig) != 1
	   )
	{ 
		return;
	}
	else
	{
		if (encontrarArista(grafo, orig, dest) == 1)
		{
			grafo->matrizAdy[orig][dest] = 0;
			grafo->numAristas = grafo->numAristas-1;
		}
		if (encontrarArista(grafo, dest, orig) == 1)
		{
			grafo->matrizAdy[dest][orig] = 0;
			grafo->numAristas = grafo->numAristas-1;
		}
	}
}

//Inestable, NO modificar
int envolverCoordenadas_X(int x)
{	
	if (x <= -1)
	{
		return 79;
	}
	else if (x >= 80)
	{
		return 0;
	}
}

//Inestable, NO modificar
int envolverCoordenadas_Y(int y)
{
	//mvprintw(51,1, "c: %d",y);
	if (y <= -1)
	{
		return 24;
	}
	else if (y >= 25)
	{
		//mvprintw(52,1, "d: %d",y);
		return 0;
	}
}

//Inestable, NO modificar
int envolverIdx_X(int idx, int lado)
{
	if (lado == 0) //Se sale de la cuadricula para la izquierda
	{
		int idxIF = idx+1;
		if ((idxIF % 80) == 0) //Verificar lo anterior
		{
			int finalDeFila = idx+80;
			return (finalDeFila);
		}
	}
	else if (lado == 1)
	{
		if (idx % 80 == 0) //Verificar lo anterior
		{
			int inicioDeFila = idx-80;
			return inicioDeFila;
		}
	}
	return idx;
}

//Inestable, NO modificar
int envolverIdx_Y(int idx)
{
	if (idx <= -1 && idx >= -80) //Se sale de la cuadricula para arriba
	{
		return 2000+idx;
	}
	else if (idx >= 2000 && idx <= 2079) //Se sale de la cuadricula para abajo
	{
		return idx-2000;
	}
	return idx;
}

int calcularAdyacencia(Elemento *vertice, Escenario *escenario, Vibora *computadora, Vibora *jugador)
{	
	//Si el vertice esta bloqueado por obstaculos entonces no es adyacente
	if (!detectarColision_Escenario(vertice->x, vertice->y, escenario))
	{
		if (!detectarColision_Vibora(vertice, jugador->cabeza, 0) &&
			!detectarColision_Vibora(vertice, computadora->cabeza, 1)
		   )
		{
			return 1;
		} 
	}
	return 0;
}

void limpiarAdyacencia(Grafo *grafo)
{
	for (int i = 0; i < 2000; i++)
	{
		for (int j = 0; j < 2000; j++)
		{
			grafoEscenario.matrizAdy[i][j] = 0;
		} 
	}
}

int obtenerVerticeAdyacente(Grafo *grafo, int idx)
{
	for (int i = 0; i < grafo->numVerticesMax; i++)
	{
		if (grafo->matrizAdy[idx][i] == 1 && grafo->vertices[i].visitado == 0)
		{
			return i;
		}
	}
	return -1;
}

int calcularVerticesAdyacentes(Grafo *grafo, int idxOrig, Escenario *escenario, Vibora *computadora, Vibora *jugador)
{
	Elemento *cabezaAdelantadaArriba = crearElemento(-1, -1, NULL);
	Elemento *cabezaAdelantadaAbajo = crearElemento(-1, -1, NULL);
	Elemento *cabezaAdelantadaIzquierda = crearElemento(-1, -1, NULL);
	Elemento *cabezaAdelantadaDerecha = crearElemento(-1, -1, NULL);
	
	int idxArriba = idxOrig - 80; //id en matriz del nodo arriba
	int idxAbajo = idxOrig + 80; //id en matriz del nodo abajo
	int idxIzquierda = idxOrig - 1; //id en matriz del nodo izquierda
	int idxDerecha = idxOrig + 1; //id en matriz del nodo derecha
	
	//Envolvemos los valores que se salen de la cuadricula por un monto especifico
	idxArriba = envolverIdx_Y(idxArriba);
	idxAbajo = envolverIdx_Y(idxAbajo);
	idxIzquierda = envolverIdx_X(idxIzquierda, 0);
	idxDerecha = envolverIdx_X(idxDerecha, 1);
	
	int arribaX_Escenario = idxArriba % 80; //x del nodo arriba del nodo pasado en el escenario
	int arribaY_Escenario = idxArriba / 80; //y del nodo arriba del nodo pasado en el escenario
	int abajoX_Escenario = idxAbajo % 80; //x del nodo abajo del nodo pasado en el escenario
	int abajoY_Escenario = idxAbajo / 80; //y del nodo abajo del nodo pasado en el escenario
	int izquierdaX_Escenario = idxIzquierda % 80; //x del nodo a la izquierda del nodo pasado en el escenario
	int izquierdaY_Escenario = idxIzquierda / 80; //y del nodo a la izquierda del nodo pasado en el escenario
	int derechaX_Escenario = idxDerecha % 80; //x del nodo a la derecha del nodo pasado en el escenario
	int derechaY_Escenario = idxDerecha / 80; //y del nodo a la derecha del nodo pasado en el escenario*/
	
	cabezaAdelantadaArriba->x = arribaX_Escenario;
	cabezaAdelantadaArriba->y = arribaY_Escenario;
	
	cabezaAdelantadaAbajo->x = abajoX_Escenario;
	cabezaAdelantadaAbajo->y = abajoY_Escenario;
	
	cabezaAdelantadaIzquierda->x = izquierdaX_Escenario;
	cabezaAdelantadaIzquierda->y = izquierdaY_Escenario;
	
	cabezaAdelantadaDerecha->x = derechaX_Escenario;
	cabezaAdelantadaDerecha->y = derechaY_Escenario;
	
	//Si el camino esta bloqueado por obstaculos entonces no es adyacente
	if (idxArriba >= 0 && idxArriba <= 1999)
	{
		if (calcularAdyacencia(cabezaAdelantadaArriba, escenario, computadora, jugador) == 1)
		{
			if (!encontrarArista(grafo, idxOrig, idxArriba))
			{
				agregarArista(grafo, idxOrig, idxArriba);
			}
		}
		else
		{
			eliminarArista(grafo, idxOrig, idxArriba);
		}
	}
	if (idxAbajo >= 0 && idxAbajo <= 1999)
	{
		if (calcularAdyacencia(cabezaAdelantadaAbajo, escenario, computadora, jugador) == 1)
		{
			if (!encontrarArista(grafo, idxOrig, idxAbajo))
			{
				agregarArista(grafo, idxOrig, idxAbajo);
			}
		}
		else
		{
			eliminarArista(grafo, idxOrig, idxAbajo);
		}
	}
	if (idxIzquierda >= 0 && idxIzquierda <= 1999)
	{
		if (calcularAdyacencia(cabezaAdelantadaIzquierda, escenario, computadora, jugador) == 1)
		{
			if (!encontrarArista(grafo, idxOrig, idxIzquierda))
			{
				agregarArista(grafo, idxOrig, idxIzquierda);
			}
		}
		else
		{
			eliminarArista(grafo, idxOrig, idxIzquierda);
		}
	}
	if (idxDerecha >= 0 && idxDerecha <= 1999)
	{
		if (calcularAdyacencia(cabezaAdelantadaDerecha, escenario, computadora, jugador) == 1)
		{
			if (!encontrarArista(grafo, idxOrig, idxDerecha))
			{
				agregarArista(grafo, idxOrig, idxDerecha);
			}
		}
		else
		{
			eliminarArista(grafo, idxOrig, idxDerecha);
		}
	}
	
	return 0;
}

int BFS(Grafo *grafo, ListaEspera *lista, Vibora *computadora, Elemento *manzanaObj)
{
	//Resetear la bandera de los vertices visitados
	for (int i = 0; i < 2000; i++)
	{
		grafo->vertices[i].visitado = 0;
	}
	
	int idxInicial = XY_a_Idx(manzanaObj->x, manzanaObj->y); 
	int idxFinal = XY_a_Idx(computadora->cabeza->x, computadora->cabeza->y);
	
	lista->vertices = grafo->vertices;
	
	//mvprintw(1,80,"manz: '%d' | vibora: %d", idxInicial, idxFinal);
	
	//Agregar el nodo inicial a la lista de espera
	//y marcarlo como visitado
	grafo->vertices[idxInicial].visitado = 1;
	agregarListaEspera(lista, lista->vertices[idxInicial]);
	
	Vertice nodoActual;
	
	//Buscar en la lista de espera si no esta vacia
	while (!listaEsperaVacia(lista))
	{
		//Obtener el vertice que esta al principio de la lista de espera
		//y buscar entre sus hojas
		nodoActual = quitarListaEspera(lista);
		
		//Iterar por los nodos adyacentes
		for (int i = 0; i < 2000; i++)
		{
			//mvprintw(6+i,80,"id:'%d'", nodoActual.idx);
			//Si el nodo adyacente es una hoja
			if (grafo->matrizAdy[nodoActual.idx][i] == 1 && lista->vertices[i].visitado == 0)
			{
				lista->vertices[i].visitado = 1;
				lista->vertices[i].anterior = nodoActual.idx;
				agregarListaEspera(lista, lista->vertices[i]);
			}
		}
		
		//Parar si el nodo actual es el nodo destino
		if (nodoActual.idx == idxFinal) { break; }
	}
	
	//El siguiente movimiento de la vibora debera ser el primer nodo
	//de la lista en reversa, por eso se hace el camino desde la manzana

	int sigMovimiento = nodoActual.idx;
	
	//mvprintw(5,80,"F:'%d' Fant:'%d'", idxFinal, nodoActual.idx);
	//mvprintw(6,80,"sig:'%d'", sigMovimiento);
	
	return sigMovimiento;
}

void crearMatAdyEscenario()
{	
	initGrafo(&grafoEscenario, 2000); //25x80, 2000 vertices
	crearListaEspera(&listaEspera, 2000);
	
	//Agregar los bloques del escenario como vertices en un grafo
	int contadorVertices = 0;
	for (int x = 0; x < 25; x++)
	{
		for (int y = 0; y < 80; y++)
		{
			agregarVertice(&grafoEscenario, contadorVertices);
			contadorVertices++;
		}
	}
	
	grafoInicializado = 1;
}

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

void alinearSigMovimiento(int idxDeseado, Vibora *computadora, Elemento *manzanaObj)
{
	int destY = idxDeseado % 80; //x del nodo pasado en el escenario
	int destX = idxDeseado / 80; //y del nodo pasado en el escenario
	
	int alineadoX = 0;
	int alineadoY = 0;
	
	destX = manzanaObj->x;
	destY = manzanaObj->y;
	
	//Si esta alineada la vibora
	if (destX == computadora->cabeza->x)
	{ alineadoX = 1; }		
	if (destY == computadora->cabeza->y)
	{ alineadoY = 1; }
	
	if (!alineadoX)
	{	
		//Si la manzana se encuentra a la derecha de la posicion
		//actual de la vibora de la computadora
		if (destX > computadora->cabeza->x)
		{
			//Entonces nos tenemos que alinear con el eje horizontal
			//Y dirigirnos a la derecha
			if (computadora->direccion != derecha)
			{ computadora->direccion = calcularRedireccionamiento(computadora->direccion, derecha); }
		}
		
		//Si la manzana se encuentra a la izquierda de la posicion
		//actual de la vibora de la computadora
		if (destX < computadora->cabeza->x)
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
		if (destY < computadora->cabeza->y)
		{
			//Entonces nos tenemos que alinear con el eje vertical
			//Y dirigirnos hacia arriba
			if (computadora->direccion != arriba)
			{ computadora->direccion = calcularRedireccionamiento(computadora->direccion, arriba); }
		}
		
		//Si la manzana se encuentra hacia abajo de la posicion
		//actual de la vibora de la computadora
		if (destY > computadora->cabeza->y)
		{
			//Entonces nos tenemos que alinear con el eje vertical
			//Y dirigirnos hacia abajo
			if (computadora->direccion != abajo)
			{ computadora->direccion = calcularRedireccionamiento(computadora->direccion, abajo); }
		}
		
		return;
	}
}

//Analiza el juego y controla a la vibora de la computadora
void analizarJuego(Vibora *computadora, Vibora *jugador, Elemento *manzanaObj, Escenario *escenario)
{
	if (!grafoInicializado) { crearMatAdyEscenario(); }
	
	for (int i = 0; i < 2000; i++)
	{
		calcularVerticesAdyacentes(&grafoEscenario, i, escenario, computadora, jugador);
	}
	int sigMovimiento = BFS(&grafoEscenario, &listaEspera, computadora, manzanaObj);
	alinearSigMovimiento(sigMovimiento, computadora, manzanaObj);
}
