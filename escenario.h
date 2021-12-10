#ifndef __MAPA__
#define __MAPA__

typedef struct ElementoEscenario
{
	char id[8];
	char peso;
	char caminos;
} ElementoEscenario;

typedef struct Escenario
{
	int filas;
	int columnas;
	ElementoEscenario **elementos;
} Escenario;

//Inicializa la estructura apuntada por el apuntador escenario
void initEscenario(Escenario *escenario);

//Obtiene memoria para almacenar una pantalla de tamano f x c
void definirEscenario(Escenario *escenario, int f, int c);

//Abre el archivo del nivel seleccionado y lo almacena en la estructura
//apuntada por el apuntador escenario
int cargarEscenario(Escenario *escenario, char *nombreNivel);

//Libera la memoria asociada a la estructura apuntada por escenario
void liberaEscenario(Escenario *escenario);

//Imprime el escenario
void imprimirEscenario(Escenario *escenario, int limiteX, int limiteY);

#endif