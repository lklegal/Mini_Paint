#include <stdio.h>
#include <stdlib.h>
#include "cabecalhos.h"
#include <GL/glut.h>

struct UtilidadesGlobais ug;

void adicionarPonto(int x, int y){
	obj *novoObjetoPonto = adicionarObjeto(ug.listaDeObjetos);
	adicionarVertice(ug.listaDeObjetos, novoObjetoPonto, ((float)x)/4.0, (600-(float)y)/4.0);
	novoObjetoPonto->incompleto = 0;
	glutPostRedisplay();
}

void adicionarReta(int x, int y){
	if(ug.objetoSendoCriado == NULL){
		ug.objetoSendoCriado = adicionarObjeto(ug.listaDeObjetos);
		ug.podeExibirMenu = 0;
		adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600-(float)y)/4.0);
	}else{
		adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600-(float)y)/4.0);
		ug.objetoSendoCriado->incompleto = 0;
		ug.podeExibirMenu = 1;
		ug.objetoSendoCriado = NULL;
	}
	HabilitarDesabilitarMenu();
	glutPostRedisplay();
}

void adicionarPoligono(int x, int y, char ultimoVertice){
	if(ug.objetoSendoCriado == NULL){
		ug.objetoSendoCriado = adicionarObjeto(ug.listaDeObjetos);
		ug.podeExibirMenu = 0;
		adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600-(float)y)/4.0);
	}else{
		if(ultimoVertice){
			ug.objetoSendoCriado->incompleto = 0;
			ug.podeExibirMenu = 1;
			ug.objetoSendoCriado = NULL;
		}else{
			adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600-(float)y)/4.0);
		}
	}
	HabilitarDesabilitarMenu();
	glutPostRedisplay();
}

char pickPonto(float px, float py, float mx, float my, float tolerancia){
	if(mx <= px+tolerancia && mx >= px-tolerancia){
		if(my <= py+tolerancia && my >= py-tolerancia){
			return 1;
		}
	}
	return 0;
}

void pickRetaCodificar(char vetorCod[4], float px, float py, float mx, float my, float tolerancia){
	vetorCod[0] = px < mx - tolerancia;
	vetorCod[1] = px > mx + tolerancia;
	vetorCod[2] = py < my - tolerancia;
	vetorCod[3] = py > my + tolerancia;
}

char pickRetaCasosTriviais(char codV1[4], char codV2[4]){
	//caso trivial 1
	if(!(codV1[0] || codV1[1] || codV1[2] || codV1[3]) || !(codV2[0] || codV2[1] || codV2[2] || codV2[3])){
		return 1;
	}

	//caso trivial 2
	if((codV1[0] && codV2[0]) || (codV1[1] && codV2[1]) || (codV1[2] && codV2[2]) || (codV1[3] && codV2[3])){
		return 0;
	}
	return 2;
}

char pickReta(vertice *v1, vertice *v2, float mx, float my, float tolerancia){
	char codV1[4] = {0, 0, 0, 0};
	char codV2[4] = {0, 0, 0, 0};

	pickRetaCodificar(codV2, v2->x, v2->y, mx, my, tolerancia);
	//para solução: 1 está dentro, 0 está fora, 2 indefinido
	char solucao = 2;

	float x_cabinha = v1->x;
	float y_cabinha = v1->y;

	do
	{
		/*na primeira iteração não cairá em nenhuma das 4 condições, já que codV1 ainda não
		foi codificado, portanto ainda está com sua codificação padrão de 0000*/
		if(codV1[0]){//se estiver à esquerda, vai para a direita
			x_cabinha = mx - tolerancia;
			y_cabinha = ((v2->y - v1->y) / (v2->x - v1->x)) * (x_cabinha - v1->x) + v1->y;
		}else if(codV1[1]){//se estiver à direita, vai para a esquerda
			x_cabinha = mx + tolerancia;
			y_cabinha = ((v2->y - v1->y) / (v2->x - v1->x)) * (x_cabinha - v1->x) + v1->y;
		}else if(codV1[2]){//se estiver abaixo, vai para cima
			y_cabinha = my - tolerancia;
			x_cabinha = ((v2->x - v1->x) / (v2->y - v1->y)) * (y_cabinha - v1->y) + v1->x;
		}else if(codV1[3]){//se estiver acima, vai para baixo
			y_cabinha = my + tolerancia;
			x_cabinha = ((v2->x - v1->x) / (v2->y - v1->y)) * (y_cabinha - v1->y) + v1->x;
		}

		//codificando com base na posição do cabinha que corre
		pickRetaCodificar(codV1, x_cabinha, y_cabinha, mx, my, tolerancia);
		solucao = pickRetaCasosTriviais(codV1, codV2);
	} while(solucao == 2);
	
	return solucao;
}

vertice** escolherAlgo(int mx, int my){
	vertice** retornos = (vertice**)malloc(sizeof(vertice*));
	retornos[0] = NULL;
	retornos[1] = NULL;
	obj *aux = *(ug.listaDeObjetos);
	vertice *aux2;
	char escolheu = 0;
	while(aux != NULL){
		aux2 = *(aux->vertices);
		while(aux2 != NULL){
			if(ug.selecionandoPonto){
				escolheu = pickPonto(aux2->x, aux2->y, ((float)mx)/4.0, (600-(float)my)/4.0, 2.0);
				if(escolheu){
					printf("Escolheu o ponto (%.1f, %.1f)\n", aux2->x, aux2->y);
					retornos[0] = aux2;
					return retornos;
				}
			}
			if(ug.selecionandoReta){
				if(aux2->prox != NULL){
					escolheu = pickReta(aux2, aux2->prox, ((float)mx)/4.0, (600-(float)my)/4.0, 2.0);
				}else if(aux->poligono){
					escolheu = pickReta(aux2, *(aux->vertices), ((float)mx)/4.0, (600-(float)my)/4.0, 2.0);
				}
				if(escolheu){
					if(aux2->prox != NULL){
						printf("Escolheu a reta de vértices (%.1f, %.1f), (%.1f, %.1f)\n", aux2->x, aux2->y,
						aux2->prox->x, aux2->prox->y);
						retornos[0] = aux2;
						retornos[1] = aux2->prox;
						return retornos;
					}else if(aux->poligono){
						printf("Escolheu a reta de vértices (%.1f, %.1f), (%.1f, %.1f)\n", aux2->x, aux2->y,
						(*(aux->vertices))->x, (*(aux->vertices))->y);
						retornos[0] = aux2;
						retornos[1] = *(aux->vertices);
						return retornos;
					}
				}
			}
			if(ug.selecionandoPoligono){
				if(!aux->poligono){
					break;
				}
			}
			aux2 = aux2->prox;
		}
		aux = aux->prox;
	}
	return retornos;
}

void clique(int botao, int clicouOuSoltou, int x, int y) {
    if (botao == GLUT_LEFT_BUTTON && clicouOuSoltou == GLUT_DOWN) {
        //printf("Clique do mouse em (%d, %d)\n", x, y);
		if(ug.adicionandoPonto){
			adicionarPonto(x, y);
		}else if(ug.adicionandoReta){
			adicionarReta(x, y);
		}else if(ug.adicionandoPoligono){
			adicionarPoligono(x, y, 0);
		}else if(ug.selecionandoPonto || ug.selecionandoReta || ug.selecionandoPoligono){
			escolherAlgo(x, y);
		}
    }
	if (botao == GLUT_LEFT_BUTTON && clicouOuSoltou == GLUT_UP) {
        //printf("Botão do mouse solto em (%d, %d)\n", x, y);
    }
	if (botao == GLUT_RIGHT_BUTTON && clicouOuSoltou == GLUT_DOWN) {
        if(ug.adicionandoPoligono){
			//verifica se o objeto já foi adicionado à lista (e, de quebra, se tem pelo menos 1 vértice)
			if(ug.objetoSendoCriado != NULL){
				//verifica se o objeto tem pelo menos 2 vértices
				if((*(ug.objetoSendoCriado->vertices))->prox != NULL){
					//verifica se o objeto tem pelo menos 3 vértices (desculpa a gambiarra)
					if((*(ug.objetoSendoCriado->vertices))->prox->prox != NULL){
						adicionarPoligono(x, y, 1);
					}
				}
			}
		}
    }
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(0.0, 0.0, 0.0);
	obj *aux = *(ug.listaDeObjetos);
	vertice *aux2;
	while(aux != NULL){
		aux2 = *(aux->vertices);
		if(aux->ponto){
			glBegin(GL_POINTS);
				while(aux2 != NULL){
					glVertex2f(aux2->x, aux2->y);
					aux2 = aux2->prox;
				}
			glEnd();
		}else if(aux->reta){
			if(aux->incompleto){
				glBegin(GL_POINTS);
					glVertex2f(aux2->x, aux2->y);
				glEnd();
			}else{
				glBegin(GL_LINES);
					while(aux2 != NULL){
						glVertex2f(aux2->x, aux2->y);
						aux2 = aux2->prox;
					}
				glEnd();
			}
		}else{
			if(aux->incompleto && aux2->prox == NULL){
				glBegin(GL_POINTS);
					glVertex2f(aux2->x, aux2->y);
				glEnd();
			}else if(aux->incompleto && aux2->prox != NULL){
				while(aux2->prox != NULL){
					glBegin(GL_LINES);
						glVertex2f(aux2->x, aux2->y);
						glVertex2f(aux2->prox->x, aux2->prox->y);
					glEnd();
					aux2 = aux2->prox;
				}
			}else if(!aux->incompleto){
				glBegin(GL_POLYGON);
					while(aux2 != NULL){
						glVertex2f(aux2->x, aux2->y);
						aux2 = aux2->prox;
					}
				glEnd();
			}
		}
		aux = aux->prox;
	}

	glutSwapBuffers();
}

int init(){
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Mini Paint");
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glPointSize(4.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
	glutDisplayFunc(display);
	glutMouseFunc(clique);
}