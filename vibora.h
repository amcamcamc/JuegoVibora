#ifndef __VIBORA__
#define __VIBORA__

enum Direccion { arriba, abajo, izquierda, derecha };

typedef struct Elemento
{
	int x;
	int y;
	char* dibujo;
	struct Elemento* sig;
} Elemento;

typedef struct Vibora
{
	char nombre[32];
	int color;
	int longitud;
	int longitudMax;
	int puntuacion;
	int viva;
	enum Direccion direccion;
	Elemento* cabeza;
} Vibora;

// Crea un elemento. en contexto de las viboras, se utiliza para 
// mover la cabeza de la vibora
Elemento *crearElemento(int x, int y, Elemento *sig);

// Elimina el ultimo punto de la vibora, se utiliza para eliminar
// los puntos rezagados de la vibora cuando se mueve
void eliminarUltimoElemento(Elemento *cabeza);

Vibora *crearVibora(char nombre[32], int color, int posX, int posY, enum Direccion direccionInicial);

// Redirecciona la vibora hacia la izquierda en direccion relativa
enum Direccion redireccionarIzquierda(enum Direccion direccionActual);

// Redirecciona la vibora hacia la derecha en direccion relativa
enum Direccion redireccionarDerecha(enum Direccion direccionActual);

// Dibuja la vibora en pantalla
void dibujarVibora(Vibora *vibora);

// Mueve la vibora de posicion
void moverVibora(Vibora *vibora, int limiteX, int limiteY);

// Detecta si la cabeza de la vibora chocaria con algun elemento de la vibora obstaculo
int detectarColision_Vibora(Elemento *cabezaDetectora, Elemento *cabezaObstaculo, int misma);

// Detecta si la cabeza de la vibora se come una manzana
int detectarColision_Manzana(Elemento *cabezaDetectora, Elemento *manzanaObj);

#endif
