#include "controles.h"

#define TECLA_ESC 27
#define TECLA_Q 113
#define TECLA_ENTER 10

#define TECLA_ARRIBA 259
#define TECLA_ABAJO 258
#define TECLA_IZQUIERDA 260
#define TECLA_DERECHA 261

#define TECLA_W 119
#define TECLA_A 97
#define TECLA_S 115
#define TECLA_D 100

int REGRESAR[2] = 
{
	TECLA_ESC,
	TECLA_Q,
};

int MOVER_ARRIBA[2] = 
{
	TECLA_ARRIBA,
	TECLA_W,
};

int MOVER_ABAJO[2] = 
{
	TECLA_ABAJO,
	TECLA_S,
};

int MOVER_IZQUIERDA[2] = 
{
	TECLA_IZQUIERDA,
	TECLA_A,
};

int MOVER_DERECHA[2] = 
{
	TECLA_DERECHA,
	TECLA_D,
};

int ACEPTAR[1] =
{
	TECLA_ENTER,
};

//Compara la tecla presionada con el arreglo de teclas de una 
//accion predeterminada y devuelve 1 o 0 dependiendo si la tecla
//es parte de la accion
int compararAccionTecla(int teclaPresionada, int accion[])
{
	int tamanoAcciones = sizeof accion / sizeof accion[0];
	for (int i = 0; i < tamanoAcciones; i++)
	{
		if (accion[i] == teclaPresionada)
		{
			return 1;
		}
	}
	return 0;
}
