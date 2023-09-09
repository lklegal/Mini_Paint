#include <stdio.h>
#include <stdlib.h>
#include "cabecalhos.h"
#include <GL/glut.h>

struct UtilidadesGlobais ug;

void adicionarPonto(int x, int y){
	obj *novoObjetoPonto = adicionarObjeto(ug.listaDeObjetos);
	novoObjetoPonto->vertices = criarListaDeVerticesDoObjeto();
	adicionarVertice(ug.listaDeObjetos, novoObjetoPonto, ((float)x)/4.0, (600-(float)y)/4.0);
	glutPostRedisplay();
}

void clique(int botao, int clicouOuSoltou, int x, int y) {
    if (botao == GLUT_LEFT_BUTTON && clicouOuSoltou == GLUT_DOWN) {
        printf("Clique do mouse em (%d, %d)\n", x, y);
		if(ug.adicionandoPonto){
			adicionarPonto(x, y);
		}
    }
	if (botao == GLUT_LEFT_BUTTON && clicouOuSoltou == GLUT_UP) {
        printf("Botão do mouse solto em (%d, %d)\n", x, y);
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

		}else{

		}
		aux = aux->prox;
	}
	
	glutSwapBuffers();
}

int init(obj **listaDeObjetos){
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

	ug.adicionandoPonto = 1;
	ug.adicionandoReta = 0;
	ug.adicionandoPoligono = 0;
	ug.listaDeObjetos = listaDeObjetos;
}