#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "copa.h"
#include "laberinto.h"

#define TOTAL_AYUDAS 10

// Pre C: El programa debe ser ejecutado sin errores de compilación.
// Post C: Se mostrarán por pantalla la presentación e instrucciones del juego.
void presentacion(){
	system("clear");
	printf("Has sido seleccionado para concursar del Torneo de los Tres Magos...\n");
	printf("Deberás recorrer el laberinto con tu jugador (J) y encontrar la Copa oculta (C) antes que tu rival (G).\n");
	printf("Para ello, dispondrás de algunas ayudas, pero también enfrentarás ciertos obtáculos.\n");
	printf("Eso sí, calcula bien tus movimientos, ¡pues cada paso que das, te resta 3 puntos de vida!\n\n");
	printf("Ayudas:\n");
	printf("Impedimenta (I) ---> Con este hechizo, no recibirás daño al caer sobre el Escreguto.\n");
	printf("Riddikulus  (R) ---> Con este hechizo, no recibirás daño al caer sobre el Boggart.\n");
	printf("Pociones    (P) ---> Al tomar una poción, recuperas 15 puntos de vida, pudiendo llegar a un máximo de 50 puntos.\n");
	printf("Esfinge     (F) ---> Al poseer la Esfinge, podrás ver la ubicación de la copa.\n\n");
	printf("Obtáculos:\n");
	printf("Escreguto   (E) ---> Si caes sobre él y no posees Impedimenta, pierdes 20 puntos de vida\n");
	printf("Boggart     (B) ---> Si caes sobre él y no posees Riddikulus, pierdes 15 puntos de vida\n");
	printf("Acromántula (A) ---> Si caes sobre ella, pierdes 10 puntos de vida\n\n");
}

// Pre C: El programa debe ser ejecutado sin errores de compilación.
// Post C: Se pedirá por pantalla la tecla necesaria a ingresar para mover al Jugador, hasta que esta sea válida.
void pedir_movimiento(juego_t juego, char *tecla){
	printf("\n\nIngrese su siguiente movimiento a realizar ('w': Arriba // 's': Abajo // 'd': Derecha // 'a': Izquiera):\n");
	printf("Considerar que el caracter ingeresado debe ser en minúscula: ");
	scanf(" %c", tecla);

	while(!es_movimiento_valido(&juego, *tecla)){
		printf("\nMovimiento inválido, no puede moverse hacia una pared. Reingrese su movimiento\n");
		scanf(" %c", tecla);
	}
}

// Pre C: La partida debe estar en curso.
// Post C: Mostrará por pantalla la vida actual del Jugador.
void imprimir_estadisticas(jugador_t jugador){
	if(jugador.vida > 0){
		printf("\nVida: %i    //   ", jugador.vida);
	}
	printf("Ayudas del Jugador: ");
	for(int i = 0; i < jugador.tope_ayudas; i++){
		printf("[%c]", jugador.ayudas[i].codigo);
	}
}

// Pre C: La partida tiene que haber finalizado, ya sea ganada o perdida.
// Post C: Mostrará por pantalla si el usuario ganó o perdió el juego.
void imprimir_resultado(juego_t juego){
	system("clear");
	if(estado_juego(juego) == 1){
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n¡¡Felicidades, has ganado el Torneo de los Tres magos!!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	}

	else if(estado_juego(juego) == -1){
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n¡Has perdido! Juego terminado.\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	}
}


int main(){
	srand((unsigned) time(NULL));
	char laberinto[TAMANIO][TAMANIO];
	char tecla;
	juego_t juego;

	presentacion();
	inicializar_laberinto(&juego);
	actualizar_laberinto(juego, laberinto);
	mostrar_laberinto(laberinto);
	imprimir_estadisticas(juego.jugador);
	
	while(estado_juego(juego) == 0){
		pedir_movimiento(juego, &tecla);
		mover_jugador(&juego.jugador, tecla);
		mover_rival(&juego);
		actualizar_juego(&juego);
		actualizar_laberinto(juego, laberinto);
		presentacion();
		mostrar_laberinto(laberinto);
		imprimir_estadisticas(juego.jugador);
	}

	imprimir_resultado(juego);

	return 0;
}