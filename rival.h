#ifndef __RIVAL__
#define __RIVAL__

#include "escenario.h"
#include "vibora.h"

typedef struct Vertice
{
   int idx;	//Id del vertice en el arreglo de vertices 1D del grafo
   int asignado; //Si el vertice existe en el grafo
   int visitado; //(Para BFS) Si el vertice ha sido visitado
   int anterior; //(Para BFS) El vertice anterior de donde se vino
} Vertice;

typedef struct ListaEspera
{
	int alFrente; //El elemento que se encuentra al frente de la lista
	int alFinal; //El elemento que se encuentra al final de la lista
	int tamano; //El tamano actual de la lista
	int tamanoMax; //El tamano maximo de la lista
	Vertice *vertices; //Arreglo de vertices dentro de la lista
} ListaEspera;

typedef struct Grafo 
{
	int **matrizAdy; //Matriz adyacencia
	int numVerticesMax; //Numero maximo de vertices a almacenarse
	int numVertices; //Número de vertices
	int numAristas; // Número de aristas
	Vertice *vertices;
} Grafo;

//Analiza el juego y controla a la vibora de la computadora
void analizarJuego(Vibora *computadora, Vibora *jugador, Elemento *manzanaObj, Escenario *escenario);

#endif
