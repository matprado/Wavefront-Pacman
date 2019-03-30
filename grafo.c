#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "fila.h"

struct grafo{
	elem m[MAX_NUM_VERTICES][MAX_NUM_VERTICES];	
	int numVertices;
	int direcionado;
};

/*Função criar_grafo: cria um grafo;
@argumentos: ponteiros para inteiros n, erro e direcionado;
@retorno: retorna, se obteve sucesso, um ponteiro para o grafo criado;	
*/
Grafo *criar_grafo(int *n, int *erro, int *direcionado){
	Grafo *G = (Grafo *)malloc(sizeof(Grafo));
	if(!G){
		*erro = 1;
		return NULL;
	}
	*erro = 0;
	int N = *n;
	if(N > MAX_NUM_VERTICES){
		*n = MAX_NUM_VERTICES;
		*erro = 1;
	}
	if(N < 0){
		*n = 0;
		*erro = 1;
	}
	G->numVertices = N;
	for(int i=0; i<MAX_NUM_VERTICES; i++)
		for(int j=0; j<MAX_NUM_VERTICES; j++)
			G->m[i][j] = SEM_ARESTA;
	G->direcionado = *direcionado;
	return G;
}

/*Função apagar_grafo: apaga um grafo;
@argumentos: ponteiros para o grafo e ponteiro para o inteiro erro;
*/
void apagar_grafo(Grafo *G, int *erro){
	if(G != NULL) free(G);
	*erro = 0;
}


/*Função inserir_aresta: cria um grafo;
@argumentos: ponteiros para inteiros n e erro;
@retorno: retorna, se obteve sucesso, um ponteiro para o grafo criado;	
*/
void inserir_aresta(Grafo *G, int *v1, int *v2, elem *n, int *erro){
	if(G == NULL || *v1 > (G->numVertices-1) || *v2 > (G->numVertices-1)) *erro = 1;
	else{
		
		G->m[*v1][*v2] = *n;
		if(!(G->direcionado)) G->m[*v2][*v1] = *n;
		*erro = 0;
	}	
}

/* Função retirar_aresta: retira uma aresta do grafo retornando a mesma.
@Argumentos: ponteiros para grafo, vértices e erro
*/
elem retirar_aresta(Grafo *G, int *v1, int *v2, int *erro){
	elem ret = SEM_ARESTA;
	if(G == NULL || *v1 > (G->numVertices-1) || *v2 > (G->numVertices-1)) *erro = 1;
	else{
		ret = G->m[*v1][*v2];
		if(ret == SEM_ARESTA) *erro = 1;
		else{
			G->m[*v1][*v2] = SEM_ARESTA;
			if(!(G->direcionado)) G->m[*v2][*v1] = SEM_ARESTA;
			*erro = 0;
		}	
	}
	return ret;	
}

/* Função verificar_aresta: verifica se dois vértices(v1->v2 se direcionado) são adjacentes.
@Argumentos: ponteiros para grafo, vértices e erro;
*/
int verificar_aresta(Grafo *G, int *v1, int *v2, int *erro){
	if(*v1 < 0 || *v2 < 0 || *v1 >= G->numVertices || *v2 >= G->numVertices){
		*erro = 1;
		return -1;
	}	
	if(G->m[*v1][*v2] != SEM_ARESTA) return 1;
	return 0;
}
/* Função imprimir_grafo: imprime o grafo representado em uma matriz de adjacência.
(mudar máscara quando mudar o elem)
@Argumento: ponteiro para grafo;
*/
void imprimir_grafo(Grafo *G){
	for(int i=0; i<G->numVertices; i++){
		for(int j=0; j<G->numVertices; j++){
			printf("%d\t", G->m[i][j]);
		}
		printf("\n");
	}
}

/*Função grafo_transposto: Cria um grafo transposto;
@Argumentos: ponteiros para grafo e erro;
*/
Grafo *grafo_transposto(Grafo *G, int *erro){
	int e;
	Grafo *aux = NULL;
	if(G != NULL && G->direcionado == 1){
		*erro = 0;
		aux = criar_grafo(&(G->numVertices), &e, &(G->direcionado));
		for(int i=0; i<G->numVertices; i++){
			for(int j=0; j<G->numVertices; j++){
				if(G->m[i][j] != SEM_ARESTA) aux->m[j][i] = G->m[i][j];
			}
		}
	}else *erro = 1;		
	return aux;
}

/*
Função lista_adj_vazia: confere se um vértice tem lista adjacente vazia ou não.
@argumentos: ponteiros para o grafo, um vértice e erro;
@retorno: retorna 1 se a lista adjacente do vertice for vazia e 0, caso contrário;
*/
int lista_adj_vazia(Grafo *G, int *v, int *erro){
	if(*v >= G->numVertices || *v < 0){
		*erro = 1;
		return 1;
	}else{
		*erro = 0;
		int aux = 0, lista_vazia = 1;
		while((aux < G-> numVertices) && (lista_vazia)){
			if(G->m[*v][aux] != SEM_ARESTA)
				lista_vazia = 0;
			else aux++;
		}
		return (lista_vazia);
	}
}

/*
Função primeiro_lista_adj: retorna o primeiro vértice adjacente ao passado como argumento;
@argumentos: ponteiros para o grafo, um vértice e erro;
@retorno: retorna o primeiro vértice adjacente ou -1 se não haver lista adjacente;
*/
int primeiro_lista_adj(Grafo *G, int *v, int *erro){
	if(*v >= G->numVertices || *v < 0) *erro = 1;
	else{
		*erro = 0;
		int aux = 0;
		if(!lista_adj_vazia(G, v, erro)){
			while(aux < G->numVertices){
				if(G->m[*v][aux] != SEM_ARESTA)	return aux;
				else aux++;
			}
		}		
	}
	return -1;
}

/* Função prox_adj: define o próximo vértice adjacente.*/
void prox_adj(Grafo *G, int *v, int *adj, elem *p, int *prox, int *fim_lista_adj, int *erro){
	if(*v > G->numVertices || *v < 0)	*erro = 1;
	else{
		*erro = 0;
		*adj = *prox;
		*p = G->m[*v][*prox];
		(*prox)++;
		while((*prox < G->numVertices) && (G->m[*v][*prox] == SEM_ARESTA)) (*prox)++;
		if(*prox >= G->numVertices) *fim_lista_adj = 1;
	}
}


/* Função aresta_menor_peso: retorna a atesta de menor peso ligada a um vértice considerando que o grafo é direcionado
@Argumentos: ponteiros para grafo e erro;
@retorno: a aresta de menor peso do grafo; 
*/
elem aresta_menor_peso(Grafo *G, int *erro){
	elem menor=-1;
	int aux=0; 
	if(G != NULL){
		for(int i=0; i<G->numVertices; i++){
			for(int j=0; j<G->numVertices; j++){
				if(G->m[i][j] != SEM_ARESTA){
					if(aux == 0){
						menor = G->m[i][j];
						aux++;
					}
					if(G->m[i][j] < menor) menor = G->m[i][j];
				}	
			}
		}
		return menor;
	}
	return -1;
}

/* Função grau_vértice: informa o grau de um vértice;
@Argumentos: ponteiros para grado e vértice;
@retorno: o grau do vértice.
*/
int grau_vertice(Grafo *G, int *v){
	int aux=0;
	for(int i=0; i<G->numVertices; i++){
		if(*v != i && G->m[*v][i] != SEM_ARESTA) aux++;
	}
	return aux;
}


/* Função proximo_adjacente_livre: Função auxiliar para o wavefront.
 Retorna o próximo vértice adjacente "livre"(que ainda não foi valorado).
@Argumentos: ponteiros para grafo e vértices;  
*/
int proximo_adjacente_livre(Grafo *G, int v, int atual){
	while(atual++ < G->numVertices){
		if(G->m[v][atual] != SEM_ARESTA && G->m[v][atual] == 0){
			return atual;
		}
	}
	return -1;
}

/*Função menor_vertice_adjacente: retorna o menor vértice adjacente.
@argumento: ponteiros para grafo e vértice
@retorno: a posição do vértice escolhido na matriz adjacente;*/
int menor_vertice_adjacente(Grafo *G, int v){
	int menorAresta = 65350;
	int menorVertice = 0;
	for(int i=0; i<G->numVertices; i++){
		if(G->m[v][i] != SEM_ARESTA && G->m[v][i] < menorAresta){
			menorAresta = G->m[v][i];
			menorVertice = i;
		}
	}
	return menorVertice;
}

/* Função wavefront: algoritmo que "espalha" sobre o grafo valores de adjacência
que crescem conforme a distância do vértice de origem;
@Argumentos: ponteiros para grafo, vértice e um inteiro com o valor base do vértice base.
*/
void wavefront(Grafo *G, int v, int valor){
	int erro = 0;
	int prox1 = -1, prox2 = -1, prox3 = -1, prox4 = -1;
	int verticeAtual = 0, valorAtual = 0;
	
	/*Cria filas para os vértices e os valores*/
	fila *vertices = fila_criar();
	fila_inserir(vertices, v);
	fila *valores = fila_criar();
	fila_inserir(valores, valor);
	
	//Enquanto a fila de vértices não estiver vazia;
	while(!fila_vazia(vertices)){

		//retira da lista o vértice e o valor e realiza o wavefront
		verticeAtual = fila_retirar(vertices);
		valorAtual = fila_retirar(valores);
		
		prox1 = proximo_adjacente_livre(G, verticeAtual, -1);
		
		//se existir próximo, aplica o valor na aresta e insere na fila
		if(prox1 >= 0){
			inserir_aresta(G, &verticeAtual, &prox1, &valorAtual, &erro);
			fila_inserir(vertices, prox1);
			fila_inserir(valores, valorAtual+1);
		}
		
		prox2 = proximo_adjacente_livre(G, verticeAtual, prox1);

		if(prox2 >= 0){
			inserir_aresta(G, &verticeAtual, &prox2, &valorAtual, &erro);
			fila_inserir(vertices, prox2);
			fila_inserir(valores, valorAtual+1);
		}

		prox3 = proximo_adjacente_livre(G, verticeAtual, prox2);

		if(prox3 >= 0){
			inserir_aresta(G, &verticeAtual, &prox3, &valorAtual, &erro);
			fila_inserir(vertices, prox3);
			fila_inserir(valores, valorAtual+1);
		}

		prox4 = proximo_adjacente_livre(G, verticeAtual, prox3);

		if(prox4 >= 0){
			inserir_aresta(G, &verticeAtual, &prox4, &valorAtual, &erro);
			fila_inserir(vertices, prox4);
			fila_inserir(valores, valorAtual+1);
		}

	}	
}