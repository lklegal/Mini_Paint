#include <stdio.h>
#include <stdlib.h>
#include "cabecalhos.h"

typedef struct{
    float dados[3][3];
}matriz;

obj** criarListaDeObjetos(){
	obj **listaDeObjetos = (obj**)malloc(sizeof(obj*));
	if(listaDeObjetos != NULL){
		*listaDeObjetos = NULL;
		return listaDeObjetos;
	}else{
		printf("Alocação falhou.");
		exit(1);
	}
}

vertice** criarListaDeVerticesDoObjeto(){
	vertice **listaDeVerticesDoObjeto = (vertice**)malloc(sizeof(vertice*));
	if(listaDeVerticesDoObjeto != NULL){
		*listaDeVerticesDoObjeto = NULL;
		return listaDeVerticesDoObjeto;
	}else{
		printf("Alocação falhou.");
		exit(1);
	}
}

obj* adicionarObjeto(obj **listaDeObjetos){
	obj *novo = (obj*)malloc(sizeof(obj));
    novo->ponto = ug.estado == CRIAR_PONTO;
    novo->reta = ug.estado == CRIAR_RETA;
    novo->poligono = ug.estado == CRIAR_POLIGONO;
    novo->incompleto = 1;
	novo->vertices = criarListaDeVerticesDoObjeto();
	novo->prox = *listaDeObjetos;
	*listaDeObjetos = novo;
    return novo;
}

void adicionarVertice(obj **listaDeObjetos, obj *objetoEspecifico, float x, float y){
    //Caso esse seja o primeiro vértice do primeiro objeto
	if(*listaDeObjetos == NULL){
		objetoEspecifico = adicionarObjeto(listaDeObjetos);
	}
    vertice **verticesDoObjeto = objetoEspecifico->vertices;
	vertice *novo = (vertice*)malloc(sizeof(vertice));
    novo->x = x;
    novo->y = y;
	novo->prox = *verticesDoObjeto;
	*verticesDoObjeto = novo;
}

void removerObjeto(obj **listaDeObjetos, int posicao){
	if(*listaDeObjetos != NULL){
		obj *aux = *listaDeObjetos;
	}
	else{
		printf("Sem objetos para remover!");
	}
}

//retorna o resultado da multiplicação de duas matrizes 3x3
float** MatMul(float a[3][3], float b[3][3]){
    //cria a matriz resultante
    float **resultado = (float**)malloc(3*sizeof(float));
    for(int i = 0; i < 3; i++){
        resultado[i] = (float*)malloc(3*sizeof(float));
    }

    float soma = 0.0;

    //multiplica as matrizes
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            soma = 0.0;
            for(int k = 0; k < 3; k++){
                soma += a[i][k] * b[k][j];
            }
            //printf("%.1f ", soma);
            resultado[i][j] = soma;
        }
    }
    return resultado;
}

//retorna o resultado da multiplicação de uma matriz 3x3 por um vetor, mas apenas o x e y do vetor são retornados
float* MatVecMul(float a[3][3], float *b){
    float soma = 0.0;
    
    //cria o vetor resultante
    float *resultado = (float*)malloc(2*sizeof(float));

    //multiplica a matriz pelo vetor
    for(int i = 0; i < 2; i++){
        soma = 0.0;
        for(int j = 0; j < 3; j++){
            soma += a[i][j] * b[j];
        }
        resultado[i] = soma;
    }
    return resultado;
}

vertice calcularCentroide(){
    vertice centroide;
    if(ug.escolhidos[1] == NULL){
        centroide.x = (ug.escolhidos[0])->x;
        centroide.y = (ug.escolhidos[0])->y;
    }else if(ug.escolhidos[0] != ug.escolhidos[1]){
        centroide.x = ((ug.escolhidos[0])->x + (ug.escolhidos[1])->x) / 2.0;
        centroide.y = ((ug.escolhidos[0])->y + (ug.escolhidos[1])->y) / 2.0;
    }else{
        int n = 0;
        float somaX = 0.0;
        float somaY = 0.0;
        vertice *aux = ug.escolhidos[0];
        while(aux != NULL){
            somaX += aux->x;
            somaY += aux->y;
            n++;
            aux = aux->prox;
        }
        centroide.x = somaX / (float)n;
        centroide.y = somaY / (float)n;
    }

    return centroide;
}

matriz construirMatriz(){
    matriz matrix;

    return matrix;
}