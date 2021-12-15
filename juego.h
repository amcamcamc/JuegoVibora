#ifndef __JUEGO__
#define __JUEGO__

//Configuracion del juego
extern int juegoActivo; //1 para continuar jugando y 0 para salir
extern int juegoVelocidad; //tiempo en microsegundos entre actualizaciones de pantalla
extern int nivel; //el nivel seleccionado
extern int finDelJuego; //se utiliza para saber el ganador del juego. 1 para el usuario, 2 para la computadora, 0 es empate

//Datos ncurses
extern int maxX; //maximo tamano X de terminal
extern int maxY; //maximo tamano Y de terminal

//Los comandos a correr cuando se carga el juego
void inicializarJuego();

//Actualiza el juego por cada cuadro
void actualizarJuego();

//Termina el juego indicando el ganador
void terminarJuego(int ganador);

//Manda a inicializar el juego y lo actualiza hasta que acabe o el usuario salga
int jugarJuego();

#endif
