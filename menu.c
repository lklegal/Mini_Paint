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
        return;
    }
    if(ug.escolhidos[0] == NULL){
        ug.estado = estadoAnterior;
        printf("Nada foi selecionado.\n");
        return;
    }
    if(opcao == DELETAR){
        ug.estado = estadoAnterior;
        return;
    }
    if((ug.escolhidos[1]) == NULL && opcao != TRANSLADAR && opcao != ROTACIONAR){
        ug.estado = estadoAnterior;
        printf("Não é possível fazer isso com pontos.\n");
        return;
    }
    switch (opcao) {
        /*
        case CRIAR_PONTO:
            ug.estado = CRIAR_PONTO;
            break;
        case CRIAR_RETA:
            ug.estado = CRIAR_RETA;
            break;
        case CRIAR_POLIGONO:
            ug.estado = CRIAR_POLIGONO;
            break;
        case SELECIONAR_PONTO:
            ug.estado = SELECIONAR_PONTO;
            break;
        case SELECIONAR_RETA:
            ug.estado = SELECIONAR_RETA;
            break;
        case SELECIONAR_POLIGONO:
            ug.estado = SELECIONAR_POLIGONO;
            break;
        */
        /*
        case TRANSLADAR:
            break;
        case ROTACIONAR:
            ug.estado = ROTACIONAR;
            break;
        */
        case ESCALAR:
            break;
        case ESPELHAR_X:
            ug.estado = estadoAnterior;
            break;
        case ESPELHAR_Y:
            ug.estado = estadoAnterior;
            break;
        case CISALHAR_X:
            ug.estado = CISALHAR_X;
            break;
        case CISALHAR_Y:
            ug.estado = CISALHAR_Y;
            break;
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