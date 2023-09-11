#include <GL/glut.h>
#include <stdio.h>
#include "cabecalhos.h"

void menuPrincipalCallback(int opcao) {
    char estadoAnterior = ug.estado;
    ug.estado = opcao;
    if(opcao == CRIAR_PONTO || opcao == CRIAR_RETA || opcao == CRIAR_POLIGONO
    || opcao == SELECIONAR_PONTO || opcao == SELECIONAR_RETA || opcao == SELECIONAR_POLIGONO){
        ug.escolhidos[0] = NULL;
        ug.escolhidos[1] = NULL;
        ug.objetoSelecionado = NULL;
        return;
    }
    if(ug.escolhidos[0] == NULL){
        ug.estado = estadoAnterior;
        printf("Nada foi selecionado.\n");
        return;
    }
    if((ug.escolhidos[1]) == NULL && opcao != TRANSLADAR && opcao != ROTACIONAR && opcao != DELETAR){
        ug.estado = estadoAnterior;
        printf("Não é possível fazer isso com pontos.\n");
        return;
    }
    
    if(opcao == ESPELHAR_X || opcao == ESPELHAR_Y){
        ug.estado = estadoAnterior;
        float eixoEspelhamento = 1.0;
        if(ug.estado == ESPELHAR_Y){
            eixoEspelhamento = -1.0;
        }
        float segundaMatriz[3][3];

        segundaMatriz[0][0] = -1.0*eixoEspelhamento;
        segundaMatriz[0][1] = 0.0;
        segundaMatriz[0][2] = 0.0;
        segundaMatriz[1][0] = 0.0;
        segundaMatriz[1][1] = 1.0*eixoEspelhamento;
        segundaMatriz[1][2] = 0.0;
        segundaMatriz[2][0] = 0.0;
        segundaMatriz[2][1] = 0.0;
        segundaMatriz[2][2] = 1.0;

        vertice centroide = calcularCentroide();

        float primeiraMatriz[3][3] = {
            {1.0, 0.0, centroide.x},
            {0.0, 1.0, centroide.y},
            {0.0, 0.0, 1.0}
        };

        float matrizIntermediaria[3][3];
        float terceiraMatriz[3][3] = {
            {1.0, 0.0, -centroide.x},
            {0.0, 1.0, -centroide.y},
            {0.0, 0.0, 1.0}
        };
        float matrizResultante[3][3];

        MatMul(primeiraMatriz, segundaMatriz, matrizIntermediaria);
        MatMul(matrizIntermediaria, terceiraMatriz, matrizResultante);
        casosMatVecMul(matrizResultante);
    }

    if(opcao == DELETAR){
        ug.estado = estadoAnterior;
        if(!((ug.objetoSelecionado)->ponto) && ug.escolhidos[1] == NULL){
            ug.escolhidos[0] = NULL;
            ug.escolhidos[1] = NULL;
            printf("Apenas pontos isolados podem ser deletados.\n");
            return;
        }
        if((ug.objetoSelecionado)->poligono && ug.escolhidos[1] != ug.escolhidos[0]){
            ug.escolhidos[0] = NULL;
            ug.escolhidos[1] = NULL;
            printf("Apenas linhas isoladas podem ser deletadas.\n");
            return;
        }
        ug.escolhidos[0] = NULL;
        ug.escolhidos[1] = NULL;
        removerObjeto();
        glutPostRedisplay();
    }
}

void criarMenu() {
    int menuPrincipal = glutCreateMenu(menuPrincipalCallback);
    glutAddMenuEntry("Add dot", CRIAR_PONTO);
    glutAddMenuEntry("Add line", CRIAR_RETA);
    glutAddMenuEntry("Add polygon", CRIAR_POLIGONO);
    glutAddMenuEntry("Select dot", SELECIONAR_PONTO);
    glutAddMenuEntry("Select line", SELECIONAR_RETA);
    glutAddMenuEntry("Select polygon", SELECIONAR_POLIGONO);
    glutAddMenuEntry("Translate", TRANSLADAR);
    glutAddMenuEntry("Rotate", ROTACIONAR);
    glutAddMenuEntry("Scale", ESCALAR);
    glutAddMenuEntry("Mirror X", ESPELHAR_X);
    glutAddMenuEntry("Mirror Y", ESPELHAR_Y);
    glutAddMenuEntry("Shear X", CISALHAR_X);
    glutAddMenuEntry("Shear Y", CISALHAR_Y);
    glutAddMenuEntry("Delete", DELETAR);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    //glutPostRedisplay();
}

void HabilitarDesabilitarMenu(){
    if(ug.podeExibirMenu){
        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }else{
        glutDetachMenu(GLUT_RIGHT_BUTTON);
    }
}