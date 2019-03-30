/*
Atividade Extra de Grafos : PACMAN SIMPLES;
 	
 	Este programa trata um jogo simples de pacman com um mapa de n por m posições.
 
 	As posições do mapa são tratadas como vértices de um grafo enquanto que as ligações
 entre essas posições(que permitem o jogador, ou o inimigo, andar) são representadas
 como as arestas.

	Desse modo, são sorteadas posições aleatórias para o pacman e o fantasma e, em sequência
 um algoritmo de IA wavefront é utilizado para que o pacman escolha o melhor(ou menor) caminho
 possível até que chegue no fantasma(que se movimenta aleatoriamente).
*/

#include <stdlib.h>
#include "grafo.h"
#include <stdio.h>
#include <time.h>

/*
Estrutura posicao criada para representar uma posição de um elemento no mapa do jogo.
Note que possui duas coordenadas: x e y, como representado comumente em um game;
*/
typedef struct posicao{
	int x;
	int y;
}Posicao;


/*Declaração de funções*/
void imprimir_jogo(Posicao pacman, Posicao fantasma, int n, int m);
void fantasma_caminha(Posicao *fantasma, int n, int m);

int main(){

	int n, m; //lados do tabuleiro m x n
	printf("Digite as dimensões do mapa: ");
	scanf("%d%d", &n, &m);
	getchar();
	int v = n * m; // v -> número de vértices
	int erro = 0, direcionado = 0;
	int aresta = 0; // indica que o valor inicial que representa a existência de uma aresta é 0.
	
	//grafo de n*m vertices não direcionado
	Grafo *G = criar_grafo(&v, &erro, &direcionado);

	/*Insere todas as arestas que ligam os vértices de acordo com o mapa do jogo*/
	for(int i=0; i<v; i++){
		int esquerda = i-1, direita = i+1, cima = i-m, baixo = i+m;
		//Insere conferindo se não está na borda do mapa para cada situação:
		if(i % m != 0) inserir_aresta(G, &i, &(esquerda), &aresta, &erro); //insere a esquerda
		if(i % m != (m-1)) inserir_aresta(G, &i, &(direita), &aresta, &erro); //insere a direita
		if(i >= m) inserir_aresta(G, &i, &(cima), &aresta, &erro); //insere em cima
		if(i < (m * n - n)) inserir_aresta(G, &i, &(baixo), &aresta, &erro); //insere em baixo
	}

	//escolhe um vertice para o fantasma e um para o pacman de forma aleatória:
	srand(time(NULL));
	Posicao pacman, fantasma;
	pacman.x = rand() % m;
	pacman.y = rand() % n;
	fantasma.x = rand() % m;
	fantasma.y = rand() % n;

	int aux = 1;
	int comeu = 0;
	if(pacman.x == fantasma.x && pacman.y == fantasma.y) comeu = 1;

	imprimir_jogo(pacman, fantasma, n, m);

	/*Execução do jogo em si:*/	
	while(comeu != 1){
		printf("\n--------------------------------------------------\n");
		printf("Digite ENTER para o pacman correr atrás do fantasma: ");
		printf("\n--------------------------------------------------\n");
		getchar();
		
		/*Algoritmo de wavefront é aplicado no fantasma*/
		wavefront(G, (fantasma.y * m + fantasma.x), 1);

		/*Escolhe o vértice com a menor aresta conectada ao vértice atual do pacman.
		  Ou seja, escolhe o melhor caminho possível para percorrer.*/
		int verticeEscolhido = menor_vertice_adjacente(G, (pacman.y * m + pacman.x));
		
		//printf("vertice Escolhido: %d\n", verticeEscolhido);
		
		//Define as coordenadas de acordo com a lógica do vértice no mapa	
		pacman.y = verticeEscolhido / m;
		pacman.x = verticeEscolhido % m;
		
		//Mostra o jogo atualizado
		printf("\t---PACMAN ANDANDO---\n");
		imprimir_jogo(pacman, fantasma, n, m);

		//Se não comeu, o fantasma caminha
		if(pacman.x != fantasma.x || pacman.y != fantasma.y){
			fantasma_caminha(&fantasma, n, m);
		}else comeu = 1;
		
		//Se, ainda assim, não comeu, mostra o jogo atualizado
		if(pacman.x != fantasma.x || pacman.y != fantasma.y){	
			printf("\t---FANTASMA ANDANDO---\n");
			imprimir_jogo(pacman, fantasma, n, m);
			/*Insere todas as arestas que ligam os vértices(zera tudo denovo para um próximo wavefront)*/
			aresta = 0;
			for(int i=0; i<v; i++){
				int esquerda = i-1, direita = i+1, cima = i-m, baixo = i+m;

				//Insere conferindo se não está na borda do mapa para cada situação:
				if(i % m != 0) inserir_aresta(G, &i, &(esquerda), &aresta, &erro); //insere a esquerda
				if(i % m != (m-1)) inserir_aresta(G, &i, &(direita), &aresta, &erro); //insere a direita
				if(i >= m) inserir_aresta(G, &i, &(cima), &aresta, &erro); //insere em cima
				if(i < (m * n - n)) inserir_aresta(G, &i, &(baixo), &aresta, &erro); //insere em baixo
			}
		}else{
			comeu = 1;
			printf("\t---FANTASMA ANDANDO---\n");
			imprimir_jogo(pacman, fantasma, n, m);
		}
	}
	// GAME OVER
	printf("O PACMAN COMEU O FANTASMA NA POSIÇÃO %dx%d. FIM DE JOGO!\n", pacman.y, pacman.x);	
}	

/* Função imprimir_jogo: imprime o mapa do jogo com o pacman e o fantasma
@Argumentos: Posições do pacman e do fantasma, além das dimensões n e m do mapa
*/
void imprimir_jogo(Posicao pacman, Posicao fantasma, int n, int m){
	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			if(pacman.x == j && pacman.y == i) printf(" ᗧ ");
			else if(fantasma.x == j && fantasma.y == i) printf(" ᗣ ");
			else printf(" # ");
		}
		printf("\n");
	}
	printf("\n");
}

/* Função fantasma_caminha: escolhe uma nova posição aleatoriamente para o fantasma
@Argumentos: ponteiro para a posição do fantasma e dimensões do mapa
*/
void fantasma_caminha(Posicao *fantasma, int n, int m){
	int escolha, mudou = 0;
	do{
		escolha = rand() % 5;
		switch(escolha){
			case 1: 
				if(fantasma->x != m-1){
					fantasma->x++;
					mudou = 1;
				}
				break;
			case 2:
				if(fantasma->x != 0){
					fantasma->x--;
					mudou = 1;
				}
				break;
			case 3:
				if(fantasma->y != 0){
					fantasma->y--;
					mudou = 1;
				}
				break;
			case 4:
				if(fantasma->y != n-1){
					fantasma->y++;
					mudou = 1;
				}	
				break;
		}
	}while(!mudou);
}