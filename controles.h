#ifndef __CONTROLES__
#define __CONTROLES__

//Tabla de controles. Definida en controles.c
extern int REGRESAR[2];
extern int MOVER_ARRIBA[2];
extern int MOVER_ABAJO[2];
extern int MOVER_IZQUIERDA[2];
extern int MOVER_DERECHA[2];
extern int ACEPTAR[1];

/*
 * Compara la tecla presionada con el arreglo de teclas de una 
 * accion predeterminada y devuelve 1 o 0 dependiendo si la tecla
 * es parte de la accion
 */
int compararAccionTecla(int teclaPresionada, int accion[]);

#endif
