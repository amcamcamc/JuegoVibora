#ifndef __JUEGO__
#define __JUEGO__

//Configuracion del juego
extern int juegoActivo; //1 para continuar jugando y 0 para salir
extern int juegoVelocidad; //velocidad en microsegundos de refrescamiento
extern int nivel; //el nivel seleccionado

//Datos ncurses
extern int maxX; //maximo tamano X de terminal
extern int maxY; //maximo tamano Y de terminal

void inicializarJuego();
void actualizarJuego();
int jugarJuego(int nivelSeleccionado, int velocidadSeleccionada);

#endif
