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
		//printf("renglon: %d\n", i);
		elementosEsc[i] = (ElementoEscenario *) malloc(c*sizeof(ElementoEscenario));
	}
	
	escenario->elementos = elementosEsc;
}

//Abre el archivo del nivel seleccionado y lo almacena en la estructura
//apuntada por el apuntador escenario
int cargarEscenario(Escenario *escenario, char *nombreNivel)
{
	//printf("\n\ncargaPantalla\n\n");
	FILE *arch;
	size_t f, c;
	char buff[512], *ptr, *tmp;
	
	arch = fopen(nombreNivel, "r");
	if (arch == NULL)
		return -1;//El error -1 indica que no se pudo leer el archivo que define la pantalla.
	
	//Leemos la primera linea del archivo, la que contiene las dimensiones
	//de la pantalla definida en el archivo y extrae las dimensiones.
	//
	//Aquí ocupamos una funcion que no hemos visto aun, strtok_r, pero la explicaremos despues.
	fgets(buff, 512, arch);
	ptr = strtok_r(buff, " \n", &tmp);
	c = (size_t)atol(ptr);
	ptr = strtok_r(NULL, " \n", &tmp);
	f = (size_t)atol(ptr);
	
	definirEscenario(escenario, f, c);
	
	// Leer cada una de las f lineas de archivo
	// y a partir de cada uno de los 2*c caracteres del renglon, inicializar cada
	// uno del los elementos del renglon correspondiente de la pantalla apuntada 
	// por escenario.
	
	char leido; 			//el caracter actualmente leido del archivo
	int posicionChar = 0;	//0 para tipo, 1 para color
	int escrito = 0;		//si ya se escribio ya sea tipo o color en esta vuelta
	int filActual = 0;		//fila actual
	int colActual = 0;		//columna actual
	
	//Leer linea por linea el archivo
	//Sabiendo que se comienza primero por el tipo, entonces asignamos
	//el tipo y despues ponemos una variable para saber que ya lo asignamos
	//ya que necesitamos pasar de nuevo por el while para cambiar de caracter
	//en el archivo. Cuando se hayan leido tipo y color, entonces aumentar
	//el contador de columna para pasar al siguiente elemento de pantalla.
	while((leido = fgetc(arch)) != EOF && !(filActual > escenario->filas))
	{
		//printf("Renglon: %d | Columna: %d\n", renActual,colActual);
		
		if (colActual >= escenario->columnas)
		{
			//printf("%d = %d\n", colActual, pnt->col);
			filActual++;
			colActual = 0;
		}
		else
		{
			//printf("Valor en pantalla: %c\n",leido);
			escenario->elementos[filActual][colActual].peso = leido;
			colActual++;
			//printf("Valor escrito: %c \n",pnt->P[renActual][colActual].Tipo);
		}
	}
	
	fclose(arch);
	return 0; //Si llegamos al final, la ejecución tuvo exito y regresamos 0;
}

//Libera la memoria asociada a la estructura apuntada por escenario
void liberaEscenario(Escenario *escenario)
{
	//printf("\n\nliberaPantalla\n\n");
	
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
