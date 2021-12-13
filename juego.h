#ifndef __JUEGO__
#define __JUEGO__

//Configuracion del juego
extern int juegoActivo; //1 para continuar jugando y 0 para salir
extern int juegoVelocidad; //velocidad en microsegundos de refrescamiento
extern int nivel; //el nivel seleccionado
extern int finDelJuego; //se utiliza para saber el ganador del juego. 1 para el usuario, 2 para la computadora, 0 es empate

//Datos ncurses
extern int maxX; //maximo tamano X de terminal
extern int maxY; //maximo tamano Y de terminal

void inicializarJuego();
void actualizarJuego();
void terminarJuego();
int jugarJuego();

#endif
