#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "escenario.h"

char *strtok_r(char *str, const char *delim, char **save)
{
	char *res, *last;
	
	if( !save )
		return strtok(str, delim);
	if( !str && !(str = *save) )
		return NULL;
	last = str + strlen(str);
	if( (*save = res = strtok(str, delim)) )
	{
		*save += strlen(res);
		if( *save < last )
			(*save)++;
		else
			*save = NULL;
	}
	return res;
}

//Detecta si las coordenadas pasadas chocarian con algun elemento del escenario
int detectarColision_Escenario(int posX, int posY, Escenario *escenario)
{
	for (int i = 0; i < escenario->filas; i++)
	{
		for (int j = 0; j < escenario->columnas; j++)
		{
			if (posX == j && posY == i)
			{
				if (escenario->elementos[i][j].peso == '0') { return 1; }
			}
		}
	}
	return 0;
}

//Inicializa la estructura apuntada por el apuntador escenario
void initEscenario(Escenario *escenario)
{
	escenario = NULL;
	escenario = malloc(sizeof(Escenario));
}

//Obtiene memoria para almacenar una pantalla de tamano f x c
void definirEscenario(Escenario *escenario, int f, int c)
{
	escenario->filas = f;
	escenario->columnas = c;
	
	ElementoEscenario **elementosEsc;
	elementosEsc = (ElementoEscenario **) malloc(f*sizeof(elementosEsc));
	
	for (int i = 0; i < f; i++)
	{
		elementosEsc[i] = (ElementoEscenario *) malloc(c*sizeof(ElementoEscenario));
	}
	
	escenario->elementos = elementosEsc;
}

//Abre el archivo del nivel seleccionado y lo almacena en la estructura
//apuntada por el apuntador escenario
int cargarEscenario(Escenario *escenario, char *nombreNivel)
{
	FILE *arch;
	size_t f, c;
	char buff[512], *ptr, *tmp;
	
	arch = fopen(nombreNivel, "r");
	if (arch == NULL)
		return -1;//El error -1 indica que no se pudo leer el archivo que define la pantalla.
	
	//Leemos la primera linea del archivo, la que contiene las dimensiones
	//de la pantalla definida en el archivo y extrae las dimensiones.
	fgets(buff, 512, arch);
	ptr = strtok_r(buff, " \n", &tmp);
	c = (size_t)atol(ptr);
	ptr = strtok_r(NULL, " \n", &tmp);
	f = (size_t)atol(ptr);
	
	definirEscenario(escenario, f, c);
	
	/* Leer cada una de las f lineas de archivo
	 * y a partir de cada uno de los 2*c caracteres del renglon, inicializar cada
	 * uno del los elementos del renglon correspondiente de la pantalla apuntada 
	 * por escenario.
	 */
	
	char leido; //El caracter actualmente leido del archivo
	int filActual = 0; //Fila actual
	int colActual = 0; //Columna actual
	
	//Leer linea por linea el archivo
	while((leido = fgetc(arch)) != EOF && !(filActual > escenario->filas))
	{
		if (colActual >= escenario->columnas)
		{
			filActual++;
			colActual = 0;
		}
		else
		{
			escenario->elementos[filActual][colActual].peso = leido;
			colActual++;
		}
	}
	
	fclose(arch);
	return 0; //Si llegamos al final, la ejecución tuvo exito y regresamos 0;
}

//Libera la memoria asociada a la estructura apuntada por escenario
void liberaEscenario(Escenario *escenario)
{
	if (escenario->elementos != NULL) { free(escenario->elementos); }
	escenario->elementos = NULL;
}

//Imprime el escenario
void imprimirEscenario(Escenario *escenario, int limiteX, int limiteY)
{	
	for (int i = 0; i < escenario->filas; i++)
	{
		for (int j = 0; j < escenario->columnas; j++)
		{	
			if (escenario->elementos[i][j].peso == '0')
			{
				attron(COLOR_PAIR(6));
				mvprintw(limiteY-(limiteY-i),limiteX-(limiteX-j),"X");
			}
			else
			{
				attron(COLOR_PAIR(5));
				mvprintw(limiteY-(limiteY-i),limiteX-(limiteX-j),";");
			}
		}
	}
}
