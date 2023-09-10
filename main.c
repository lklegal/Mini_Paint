#include <stdio.h>
#include <stdlib.h>
#include "cabecalhos.h"
#include <GL/glut.h>

int main(int argc, char** argv){
    /*
    float a[3][3] = {
        {1.1, 3.5, 3.7},
        {4.3, 5.7, 9.9},
        {8.4, 2.8, 1.0}
    };
    float b[3][3] = {
        {4.1, 12.0, 7.3},
        {6.2, 5.3, 6.6},
        {1.5, 2.0, 1.0}
    };
    float **resultadoTeste = MatMul(a, b);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%f ", resultadoTeste[i][j]);
        }
        printf("\n");
    }
    */

    ug.listaDeObjetos = criarListaDeObjetos();
    ug.escolhidos = (vertice**)malloc(2*sizeof(vertice*));
    ug.estado = CRIAR_PONTO;
    ug.podeExibirMenu = 1;
    ug.transladando = 0;
    glutInit(&argc, argv);
	init();
    criarMenu();
	glutMainLoop();
	return 0;
}