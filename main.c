#include "cabecalhos.h"
#include <GL/glut.h>

int main(int argc, char** argv){
    ug.listaDeObjetos = criarListaDeObjetos();
    ug.escolhidos = (vertice**)malloc(2*sizeof(vertice*));
    ug.escolhidos[0] = NULL;
    ug.escolhidos[1] = NULL;
    ug.objetoSelecionado = NULL;
    ug.estado = CRIAR_PONTO;
    ug.podeExibirMenu = 1;
    ug.transladando = 0;
    glutInit(&argc, argv);
	init();
    criarMenu();
	glutMainLoop();
	return 0;
}