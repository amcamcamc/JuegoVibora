#ifndef __RIVAL__
#define __RIVAL__

#include "escenario.h"
#include "vibora.h"

void alinearConManzana(Elemento *manzanaObj, Vibora *computadora);

void analizarJuego(Vibora *computadora, Vibora *jugador, Elemento *manzanaObj, Escenario *escenario);

#endif
