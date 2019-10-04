#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "copa.h"
#include "laberinto.h"

// Constantes
	#define TAMANIO 15
	#define PARED '#'
	#define VACIO ' '

	#define COPA 		'C'
	#define ESCREGUTO 	'E'
	#define ACROMANTULA 'A'
	#define BOGGART 	'B'
	#define IMPEDIMENTA 'I'
	#define RIDDIKULUS  'R'
	#define POCION 		'P'
	#define ESFINGE 	'F'
	#define RIVAL 		'G'
	#define JUGADOR 	'J'
	
	#define DERECHA   'd'
	#define ABAJO 	  's'
	#define IZQUIERDA 'a'
	#define ARRIBA	  'w'

	#define POSICION_IMPEDIMENTA 0
	#define POSICION_RIDDIKULUS  1
	#define POSICION_POCION_1    2
	#define POSICION_POCION_2    3
	#define POSICION_POCION_3    4
	#define POSICION_ESFINGE     5
	
	#define POSICION_ESCREGUTO 0
	#define POSICION_ACROMANTULA 1
	#define POSICION_BOGGART 2

	#define DANIO_ESCREGUTO   20
	#define DANIO_ACROMANTULA 10
	#define DANIO_BOGGART     15

	#define VIDA_JUGADOR 		 50
	#define VIDA_PARA_VER_COPA   15
	#define VIDA_RECUPERA_POCION 15
	#define VIDA_POR_CADA_PASO 3
	
	#define FILA_MIN 0
	#define FILA_MAX 14
	#define COL_MIN 0
	#define COL_MAX 14
	
	#define SIN_PASOS_RIVAL 0
	#define CANT_MOVIMIENTOS_POR_DIRECCION 4
	
	#define TOPE_AYUDAS_INICIAL	    6
	#define TOPE_OBSTACULOS_INICIAL 3

// Pre C: Debe recibir la matriz del laberinto original válida.
// Post C: Crea una copia de las paredes del laberinto orignial en una segunda matriz laberinto.
void copiar_laberinto(char laberinto[TAMANIO][TAMANIO], char laberinto_copia[TAMANIO][TAMANIO]){
	for(int i = 0; i < TAMANIO; i++){
		for(int j = 0; j < TAMANIO; j++){
			laberinto_copia[i][j] = laberinto[i][j];
		}
	}
}

// Pre C: Se debe recibir una coordenada válida dentro de la matriz laberinto.
// Post C: Devuelve 'true' si dicha coordenada contiene una pared '#'.
bool hay_pared(char laberinto[TAMANIO][TAMANIO], int fila, int columna){
	bool pared = false;
	if(laberinto[fila][columna] == PARED){
		pared = true;
	}
	return pared;
}

// Pre C: Se debe recibir una coordenada válida dentro de la matriz laberinto.
// Post C: Devuelve 'true' si dicha coordenada contiene un espacio vacío ' '.
bool espacio_vacio(char laberinto[TAMANIO][TAMANIO], coordenada_t coordenada){
	bool vacio = false;
	if(laberinto[coordenada.fil][coordenada.col] == VACIO){
		vacio = true;
	}
	return vacio;
}

//Pre C: Se deben recibir dos coordenadas válidas de la matriz laberinto para ser comparadas.
//Post C: Devuelve 'true' si dichas coordenada recibidas son iguales.
bool coordenadas_iguales(coordenada_t coordenada_1, coordenada_t coordenada_2){
	bool son_iguales = false;
	if((coordenada_1.fil == coordenada_2.fil) && (coordenada_1.col == coordenada_2.col)){
		son_iguales = true;
	}
	return son_iguales;
}

// Pre C: Recibe una coordenada y la dirección (caracter) hacia la que se quiere mover el jugador.
// Post C: Devuelve 'true' si la dirección lleva al jugador a un rango válido de la matriz laberinto.
bool esta_dentro_de_rango(coordenada_t coordenada, char direccion){
	bool se_puede_mover = true;
	if((coordenada.col == COL_MAX) && (direccion == DERECHA)){
		se_puede_mover = false;
	}
	else if((coordenada.fil == FILA_MAX) && (direccion == ABAJO)){
		se_puede_mover = false;
	}
	else if((coordenada.col == COL_MIN) && (direccion == IZQUIERDA)){
		se_puede_mover = false;
	}
	else if((coordenada.fil == FILA_MIN) && (direccion == ARRIBA)){
		se_puede_mover = false;
	}
	return se_puede_mover;
}

/* Determinará si el caracter ingresado es válido, esto es, es el caracter ‘a’ o 
   ‘s’ o ‘d’ o ‘w’ y además el jugador puede moverse en esa dirección, o sea, hay pasillo. */
bool es_movimiento_valido(juego_t* juego, char tecla){
	bool es_valido = false;
	if(esta_dentro_de_rango(juego->jugador.posicion, tecla)){
		if((tecla == DERECHA) && (!hay_pared(juego->laberinto_original, juego->jugador.posicion.fil, juego->jugador.posicion.col+1))){
			es_valido = true;
		}
		else if((tecla == ABAJO) && (!hay_pared(juego->laberinto_original, juego->jugador.posicion.fil+1, juego->jugador.posicion.col))){
			es_valido = true;
		}
		else if((tecla == IZQUIERDA) && (!hay_pared(juego->laberinto_original, juego->jugador.posicion.fil, juego->jugador.posicion.col-1))){
			es_valido = true;
		}
		else if((tecla == ARRIBA) && (!hay_pared(juego->laberinto_original, juego->jugador.posicion.fil-1, juego->jugador.posicion.col))){
			es_valido = true;
		}
	}
	return es_valido;
}

// Pre C: Debe recibir una tecla válida entre las 4 contempladas para su debido funcionamiento.
// Post C: Modifica la coordenada en la dirección definida por la tecla recibida.
void realizar_movimiento(coordenada_t* coordenada, char direccion){
	if(direccion == DERECHA){
		coordenada->col++;
	}
	else if(direccion == ABAJO){
		coordenada->fil++;
	}
	else if(direccion == IZQUIERDA){
		coordenada->col--;
	}
	else if(direccion == ARRIBA){
		coordenada->fil--;
	}
}

/* Moverá el jugador hacia la dirección especificada (dicho movimiento es válido) */
void mover_jugador(jugador_t* jugador, char direccion){
	realizar_movimiento(&(jugador->posicion), direccion);
}

// Pre C: Debe recibir una posición válida del rival.
// Post C: Devuelve 'true' si el rival puede moverse en la dirección siguiente, según su algoritmo de movimientos (si no hay pared '#').
bool movimiento_valido_rival(char laberinto[TAMANIO][TAMANIO], rival_t rival){
	bool es_valido = false;
	if(esta_dentro_de_rango(rival.posicion, rival.direccion)){
		if((!hay_pared(laberinto, rival.posicion.fil, rival.posicion.col+1)) && (rival.direccion == DERECHA)){
			es_valido = true;
		}
		else if((!hay_pared(laberinto, rival.posicion.fil+1, rival.posicion.col)) && (rival.direccion == ABAJO)){
			es_valido = true;
		}
		else if((!hay_pared(laberinto, rival.posicion.fil, rival.posicion.col-1)) && (rival.direccion == IZQUIERDA)){
			es_valido = true;
		}
		else if((!hay_pared(laberinto, rival.posicion.fil-1, rival.posicion.col)) && (rival.direccion == ARRIBA)){
			es_valido = true;
		}
	}
	return es_valido;
}

// Pre C: El rival tiene que intentar moverse hacia una pared '#' o haber agotado sus movimientos en una dirección.
// Post C: Modifica la dirección del rival para intentar moverse en la siguiente dirección, según su algortimo programado.
void cambiar_direccion_rival(char* direccion){
	if((*direccion) == DERECHA){
		(*direccion) = ABAJO;
	}
	else if((*direccion) == ABAJO){
		(*direccion) = IZQUIERDA;
	}
	else if((*direccion) == IZQUIERDA){
		(*direccion) = ARRIBA;
	}
	else if((*direccion) == ARRIBA){
		(*direccion) = DERECHA;
	}
}

/* Moverá el rival a la próxima posición. */
void mover_rival(juego_t* juego){
	bool se_movio = false;
	while(!se_movio){
		if(movimiento_valido_rival(juego->laberinto_original, juego->rival)){
			realizar_movimiento(&(juego->rival.posicion), juego->rival.direccion);
			juego->rival.cantidad_pasos++;
			se_movio = true;
			if(juego->rival.cantidad_pasos == CANT_MOVIMIENTOS_POR_DIRECCION){
				cambiar_direccion_rival(&(juego->rival.direccion));
				juego->rival.cantidad_pasos = SIN_PASOS_RIVAL;
			}
		}
		else{
			cambiar_direccion_rival(&(juego->rival.direccion));
			juego->rival.cantidad_pasos = SIN_PASOS_RIVAL;
		}
	}
}

// Pre C.: El jugador debe caer sobre un posición válida del laberinto.
// Post C: Devuelve 'true' si la posición del jugador es igual a la de una ayuda.
bool piso_ayuda(coordenada_t posicion_jugador, coordenada_t posicion_ayuda){
	bool ayuda_pisada = false;
	if(coordenadas_iguales(posicion_jugador, posicion_ayuda)){
		ayuda_pisada = true;
	}
	return ayuda_pisada;
}

// Pre C.: El jugador debe caer sobre un posición válida del laberinto.
// Post C: Devuelve 'true' si la posición del jugador es igual a la de un obstáculo.
bool piso_obstaculo(coordenada_t posicion_jugador, coordenada_t posicion_obstaculo){
	bool obstaculo_pisado = false;
	if(coordenadas_iguales(posicion_jugador, posicion_obstaculo)){
		obstaculo_pisado = true;
	}
	return obstaculo_pisado;
}

// Pre C.: El jugador debe caer sobre un posición válida del laberinto.
// Post C: Devuelve 'true' si la ayuda pisada es una pocíon.
bool es_pocion(char ayuda_pisada){
	bool pocion = false;
	if(ayuda_pisada == POCION){
		pocion = true;
	}
	return pocion;
}

// Pre C.: El jugador debe caer sobre una ayuda que no sea de uso inmediato (no debe ser poción).
// Post C: Guarda la ayuda pisada en el vector de ayudas del jugador.
void obtener_ayudas(jugador_t* jugador, ayuda_t ayuda){
	jugador->ayudas[jugador->tope_ayudas] = ayuda;
	(jugador->tope_ayudas)++;
}

// Pre C: El jugador debe haber caído previamente sobre una ayuda que no sea poción.
// Post C: Devuelve 'true' si la ayuda buscada se encuentra en el vector de ayudas del jugador.
bool posee_ayuda(jugador_t jugador, char elemento_buscado){
	bool posee = false;
	for(int i = 0; i < jugador.tope_ayudas; i++){
		if(jugador.ayudas[i].codigo == elemento_buscado){
			posee = true;
		}
	}
	return posee;
}

// Pre C: El jugador debe realizar un movimiento válido.
// Post C: Resta la vida correspondiente a cada paso y, eventualmente, al obstáculo pisado (en caso de no tener el hechizo que lo anula).
void restar_vida_jugador(jugador_t* jugador, obstaculo_t obstaculos[TOTAL_OBSTACULOS], int tope){
	jugador->vida -= VIDA_POR_CADA_PASO;
	for(int i = 0; i < tope; i++){
		if(piso_obstaculo(jugador->posicion, obstaculos[i].posicion)){
			if((obstaculos[i].codigo == ESCREGUTO) && (!posee_ayuda(*jugador, IMPEDIMENTA))){
				jugador->vida -= obstaculos[POSICION_ESCREGUTO].danio;
			}
			else if((obstaculos[i].codigo == BOGGART) && (!posee_ayuda(*jugador, RIDDIKULUS))){
				jugador->vida -= obstaculos[POSICION_BOGGART].danio;
			}
			else if(obstaculos[i].codigo == ACROMANTULA){
				jugador->vida -= obstaculos[POSICION_ACROMANTULA].danio;
			}
		}
	}
}

// Pre C: El jugador debe caer sobre una poción.
// Post C: Se le suma la vida correspondiente (pudiendo ser un máximo de 15 puntos por cada poción, sin superar los 50 puntos de vida).
void recuperar_vida_jugador(jugador_t* jugador, int vida_a_recuperar){
	if(jugador->vida > (VIDA_JUGADOR - vida_a_recuperar)){
		jugador->vida = VIDA_JUGADOR;
	}

	else{
		jugador->vida += vida_a_recuperar;
	}
}

// Pre C: El jugador debe caer sobre una ayuda.
// Post C: La ayuda pisada se elimina de la matriz y del vector de ayudas del juego.
void eliminar_ayudas(ayuda_t ayudas[TOTAL_AYUDAS], int posicion_a_eliminar, int* tope){
	for(int i = posicion_a_eliminar; i < (*tope)-1; i++){
		ayudas[i] = ayudas[i+1];
	}
	(*tope)--;
}

// Pre C: El jugador debe caer sobre un obstáculo.
// Post C: El obstáculo pisado se elimina de la matriz y del vector de obstáculos del juego.
void eliminar_obstaculos(jugador_t jugador, obstaculo_t obstaculos[TOTAL_OBSTACULOS], int* tope){
	int i = 0;
	int posicion_a_eliminar = 0;
	bool posee_elemento = false;
	
	while((i < (*tope)) && (!posee_elemento)){
		if(piso_obstaculo(jugador.posicion, obstaculos[i].posicion)){
			posicion_a_eliminar = i;
			posee_elemento = true;
		}
		i++;
	}

	if(posee_elemento){
		for(int j = posicion_a_eliminar; j < (*tope)-1; j++){
			obstaculos[j] = obstaculos[j+1];
		}
		(*tope)--;
	}
}

// Pre C: El jugador debe caer sobre una ayuda.
// Post C: Modifica la situación de la ayuda (ya sea guardarla en el vector de ayudas del jugador o, si es poción, consumirla) y luego la elimina de la matriz.
void actualizar_ayudas(juego_t* juego){
	for(int i = 0; i < (juego->tope_ayudas); i++){
		if(piso_ayuda(juego->jugador.posicion, juego->ayudas[i].posicion)){
			if(es_pocion(juego->ayudas[i].codigo)){
				recuperar_vida_jugador(&(juego->jugador), VIDA_RECUPERA_POCION);
			}
			else{
				obtener_ayudas(&(juego->jugador), juego->ayudas[i]);
			}
			eliminar_ayudas(juego->ayudas, i, &(juego->tope_ayudas));
		}
	}
}

/* Actualizará el juego. Restará vida si el jugador está sobre un obstáculo o lo eliminará
   si cuenta con el hechizo, aprenderá hechizos y todo lo que pueda suceder luego de un turno. */
void actualizar_juego(juego_t* juego){
	restar_vida_jugador(&(juego->jugador), juego->obstaculos, juego->tope_obstaculos);
	actualizar_ayudas(juego);
	eliminar_obstaculos(juego->jugador, juego->obstaculos, &(juego->tope_obstaculos));
}

/* Actualizará la matriz mostrada al usuario, con los elementos presentes en el juego. */
void actualizar_laberinto(juego_t juego, char laberinto[TAMANIO][TAMANIO]){
	copiar_laberinto(juego.laberinto_original, laberinto);
	
	if(posee_ayuda(juego.jugador, ESFINGE) || (juego.jugador.vida <= VIDA_PARA_VER_COPA)){
		laberinto[juego.copa.posicion.fil][juego.copa.posicion.col] = juego.copa.codigo;
	}
	for(int i = 0; i < juego.tope_ayudas; i++){
		laberinto[juego.ayudas[i].posicion.fil][juego.ayudas[i].posicion.col] = juego.ayudas[i].codigo;
	}
	for(int j = 0; j < juego.tope_obstaculos; j++){
		laberinto[juego.obstaculos[j].posicion.fil][juego.obstaculos[j].posicion.col] = juego.obstaculos[j].codigo;	
	}
	laberinto[juego.rival.posicion.fil][juego.rival.posicion.col] = juego.rival.codigo;
	laberinto[juego.jugador.posicion.fil][juego.jugador.posicion.col] = juego.jugador.codigo;
}

/* Devolverá el estado del juego: 1 ganado, 0 en curso, -1 perdido. */
int estado_juego(juego_t juego){
	if(coordenadas_iguales(juego.jugador.posicion, juego.copa.posicion)){
		return 1;
	}

	else if((coordenadas_iguales(juego.rival.posicion, juego.copa.posicion)) || (juego.jugador.vida <= 0)){
		return -1;
	}

	else{
		return 0;
	}
}

/* Devolverá una coordenada aleatoria dentro del rango TAMANIOxTAMANIO.
   No valida que dicha coordenada coincida con un pasillo ni que exista otro objeto en esa posición. */
coordenada_t posicion_aleatoria(){
	coordenada_t coordenada;
	coordenada.fil = rand()%TAMANIO;
	coordenada.col = rand()%TAMANIO;

	return coordenada;
}

// Pre C: Debe recibir una coordenada dentro de la matriz.
// Post C: Devuelve 'true' si la coordenada no es pared '#' y, por ende, es un espacio vacío ' '.
bool posicion_valida(coordenada_t coordenada, char laberinto[TAMANIO][TAMANIO]){
	bool posicion = false;
	if((!hay_pared(laberinto, coordenada.fil, coordenada.col)) && (espacio_vacio(laberinto, coordenada))){
		posicion = true;
	}
	return posicion;
}

// Pre C: Debe recibir una posición válida del personaje pedido.
// Post C: Devulve 'true' si la distancia Manhattan entre el personaje y la copa es mayor estricto a 10 unidades/pasos.
bool distancia_valida(coordenada_t personaje, coordenada_t copa){
	return abs(personaje.fil - copa.fil) + abs(personaje.col - copa.col) > 10;
}

// Pre C: Debe recibir la posición de un elemento o personaje del juego.
// Post C: Asigna la posición del mismo hasta que esta sea válida.
void asignar_posicion(coordenada_t *coordenada, char laberinto[TAMANIO][TAMANIO]){
	do{
		(*coordenada) = posicion_aleatoria();
	}
	while(!posicion_valida(*coordenada, laberinto));
}

// Pre C: Recibe una copia del laberito original y la estructura copa_t (ya declarada en copa.h).
// Post C: Inicializará cada componente de dicha estructura con su valor correspondiente.
void inicializar_copa(copa_t *copa, char laberinto[TAMANIO][TAMANIO]){
	copa->codigo = COPA;
	asignar_posicion(&(copa->posicion), laberinto);
	laberinto[copa->posicion.fil][copa->posicion.col] = copa->codigo;
}

// Pre C: Recibe una copia del laberito original y la estructura juego_t (ya declarada en copa.h).
// Post C: Inicializará cada componente de la estructura obstaculo_t para cada obstáculo (y el tope_obstaculos) con su valor correspondiente.
void inicializar_obstaculos(juego_t *juego, char laberinto[TAMANIO][TAMANIO]){
	juego->tope_obstaculos = TOPE_OBSTACULOS_INICIAL;

	juego->obstaculos[POSICION_ESCREGUTO].codigo = ESCREGUTO;
	asignar_posicion(&(juego->obstaculos[POSICION_ESCREGUTO].posicion), laberinto);
	juego->obstaculos[POSICION_ESCREGUTO].danio = DANIO_ESCREGUTO;
	laberinto[juego->obstaculos[POSICION_ESCREGUTO].posicion.fil][juego->obstaculos[POSICION_ESCREGUTO].posicion.col] = juego->obstaculos[POSICION_ESCREGUTO].codigo;

	juego->obstaculos[POSICION_ACROMANTULA].codigo = ACROMANTULA;
	asignar_posicion(&(juego->obstaculos[POSICION_ACROMANTULA].posicion), laberinto);
	juego->obstaculos[POSICION_ACROMANTULA].danio = DANIO_ACROMANTULA;
	laberinto[juego->obstaculos[POSICION_ACROMANTULA].posicion.fil][juego->obstaculos[POSICION_ACROMANTULA].posicion.col] = juego->obstaculos[POSICION_ACROMANTULA].codigo;

	juego->obstaculos[POSICION_BOGGART].codigo = BOGGART;
	asignar_posicion(&(juego->obstaculos[POSICION_BOGGART].posicion), laberinto);
	juego->obstaculos[POSICION_BOGGART].danio = DANIO_BOGGART;
	laberinto[juego->obstaculos[POSICION_BOGGART].posicion.fil][juego->obstaculos[POSICION_BOGGART].posicion.col] = juego->obstaculos[POSICION_BOGGART].codigo;
}

// Pre C: Recibe una copia del laberito original y la estructura juego_t (ya declarada en copa.h).
// Post C: Inicializará cada componente de la estructura ayuda_t para cada ayuda (y el tope_ayudas) con su valor correspondiente.
void inicializar_ayudas(juego_t *juego, char laberinto[TAMANIO][TAMANIO]){
	juego->tope_ayudas = TOPE_AYUDAS_INICIAL;

	juego->ayudas[POSICION_IMPEDIMENTA].codigo = IMPEDIMENTA;
	asignar_posicion(&(juego->ayudas[POSICION_IMPEDIMENTA].posicion), laberinto);
	juego->ayudas[POSICION_IMPEDIMENTA].vida_a_recuperar = 0;
	laberinto[juego->ayudas[POSICION_IMPEDIMENTA].posicion.fil][juego->ayudas[POSICION_IMPEDIMENTA].posicion.col] = juego->ayudas[POSICION_IMPEDIMENTA].codigo;

	juego->ayudas[POSICION_RIDDIKULUS].codigo = RIDDIKULUS;
	asignar_posicion(&(juego->ayudas[POSICION_RIDDIKULUS].posicion), laberinto);
	juego->ayudas[POSICION_RIDDIKULUS].vida_a_recuperar = 0;
	laberinto[juego->ayudas[POSICION_RIDDIKULUS].posicion.fil][juego->ayudas[POSICION_RIDDIKULUS].posicion.col] = juego->ayudas[POSICION_RIDDIKULUS].codigo;

	juego->ayudas[POSICION_POCION_1].codigo = POCION;
	asignar_posicion(&(juego->ayudas[POSICION_POCION_1].posicion), laberinto);
	juego->ayudas[POSICION_POCION_1].vida_a_recuperar = VIDA_RECUPERA_POCION;
	laberinto[juego->ayudas[POSICION_POCION_1].posicion.fil][juego->ayudas[POSICION_POCION_1].posicion.col] = juego->ayudas[POSICION_POCION_1].codigo;

	juego->ayudas[POSICION_POCION_2].codigo = POCION;
	asignar_posicion(&(juego->ayudas[POSICION_POCION_2].posicion), laberinto);
	juego->ayudas[POSICION_POCION_2].vida_a_recuperar = VIDA_RECUPERA_POCION;
	laberinto[juego->ayudas[POSICION_POCION_2].posicion.fil][juego->ayudas[POSICION_POCION_2].posicion.col] = juego->ayudas[POSICION_POCION_2].codigo;

	juego->ayudas[POSICION_POCION_3].codigo = POCION;
	asignar_posicion(&(juego->ayudas[POSICION_POCION_3].posicion), laberinto);
	juego->ayudas[POSICION_POCION_3].vida_a_recuperar = VIDA_RECUPERA_POCION;
	laberinto[juego->ayudas[POSICION_POCION_3].posicion.fil][juego->ayudas[POSICION_POCION_3].posicion.col] = juego->ayudas[POSICION_POCION_3].codigo;

	juego->ayudas[POSICION_ESFINGE].codigo = ESFINGE;
	asignar_posicion(&(juego->ayudas[POSICION_ESFINGE].posicion), laberinto);
	juego->ayudas[POSICION_ESFINGE].vida_a_recuperar = 0;
	laberinto[juego->ayudas[POSICION_ESFINGE].posicion.fil][juego->ayudas[POSICION_ESFINGE].posicion.col] = juego->ayudas[POSICION_ESFINGE].codigo;
}

// Pre C: Recibe una copia del laberito original y las estructuras rival_t y copa_t (ya declaradas en copa.h).
// Post C: Inicializará cada componente de la estructura del rival con su valor correspondiente (junto con la distancia entre este y la Copa).
void inicializar_rival(rival_t *rival, copa_t* copa, char laberinto[TAMANIO][TAMANIO]){
	rival->codigo = RIVAL;
	do{
	asignar_posicion(&(rival->posicion), laberinto);
	}

	while(!distancia_valida(rival->posicion, copa->posicion));
	rival->direccion = DERECHA;
	rival->cantidad_pasos = 0;
	laberinto[rival->posicion.fil][rival->posicion.col] = rival->codigo;
}

// Pre C: Recibe una copia del laberito original y las estructuras jugador_t y copa_t (ya declaradas en copa.h).
// Post C: Inicializará cada componente de la estructura del jugador con su valor correspondiente (junto con la distancia entre este y la Copa).
void inicializar_jugador(jugador_t *jugador, copa_t* copa, char laberinto[TAMANIO][TAMANIO]){
	jugador->codigo = JUGADOR;
	jugador->vida = VIDA_JUGADOR;
	do{
		asignar_posicion(&(jugador->posicion), laberinto);
	}

	while(!distancia_valida(jugador->posicion, copa->posicion));
	jugador->tope_ayudas = 0;
}

/* Inicializará todas las estructuras con los valores correspondientes, creará el laberinto, posicionará todos los elementos, etc. */
void inicializar_laberinto(juego_t *juego){
	inicializar_paredes_laberinto(juego->laberinto_original);
	char laberinto[TAMANIO][TAMANIO];
	copiar_laberinto(juego->laberinto_original, laberinto);

	inicializar_copa(&(juego->copa), laberinto);
	inicializar_obstaculos(juego, laberinto);
	inicializar_ayudas(juego, laberinto);
	inicializar_rival(&(juego->rival), &(juego->copa), laberinto);
	inicializar_jugador(&(juego->jugador), &(juego->copa), laberinto);
}

/* Mostrará el laberinto por pantalla. */
void mostrar_laberinto(char laberinto[TAMANIO][TAMANIO]){
    for(int fila = 0; fila < TAMANIO; fila++){
        for (int columna = 0; columna < TAMANIO; columna++){
			printf("%c ", laberinto[fila][columna]);
		}
    printf("\n");
	}
}