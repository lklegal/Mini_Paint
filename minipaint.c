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
		adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600-(float)y)/4.0);
	}else{
		adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600-(float)y)/4.0);
		ug.objetoSendoCriado->incompleto = 0;
		ug.objetoSendoCriado = NULL;
	}
	glutPostRedisplay();
}

void adicionarPoligono(int x, int y, char ultimoVertice){
	if(ug.objetoSendoCriado == NULL){
		ug.objetoSendoCriado = adicionarObjeto(ug.listaDeObjetos);
		adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600-(float)y)/4.0);
	}else{
		if(ultimoVertice){
			ug.objetoSendoCriado->incompleto = 0;
			ug.objetoSendoCriado = NULL;
		}else{
			adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600-(float)y)/4.0);
		}
	}
	glutPostRedisplay();
}

void clique(int botao, int clicouOuSoltou, int x, int y) {
    if (botao == GLUT_LEFT_BUTTON && clicouOuSoltou == GLUT_DOWN) {
        //printf("Clique do mouse em (%d, %d)\n", x, y);
		if(ug.adicionandoPonto){
			adicionarPonto(x, y);
		}else if(ug.adicionandoReta){
			adicionarReta(x, y);
		}else{
			adicionarPoligono(x, y, 0);
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
	
	glColor3f(1.0, 0.0, 0.0);
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
	glPointSize(5.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
	glutDisplayFunc(display);
	glutMouseFunc(clique);

	ug.adicionandoPonto = 0;
	ug.adicionandoReta = 0;
	ug.adicionandoPoligono = 1;
}