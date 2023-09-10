#include <GL/glut.h>
#include <stdio.h>
#include "cabecalhos.h"

void menuPrincipalCallback(int opcao) {
    switch (opcao) {
        case CRIAR_PONTO:
            ug.adicionandoPonto = 1;
            ug.adicionandoReta = 0;
            ug.adicionandoPoligono = 0;
            ug.selecionandoPonto = 0;
            ug.selecionandoReta = 0;
            ug.selecionandoPoligono = 0;
            ug.transladando = 0;
            ug.rotacionando = 0;
            ug.escalando = 0;
            ug.cisalhando = 0;
            break;
        case CRIAR_RETA:
            ug.adicionandoPonto = 0;
            ug.adicionandoReta = 1;
            ug.adicionandoPoligono = 0;
            ug.selecionandoPonto = 0;
            ug.selecionandoReta = 0;
            ug.selecionandoPoligono = 0;
            ug.transladando = 0;
            ug.rotacionando = 0;
            ug.escalando = 0;
            ug.cisalhando = 0;
            break;
        case CRIAR_POLIGONO:
            ug.adicionandoPonto = 0;
            ug.adicionandoReta = 0;
            ug.adicionandoPoligono = 1;
            ug.selecionandoPonto = 0;
            ug.selecionandoReta = 0;
            ug.selecionandoPoligono = 0;
            ug.transladando = 0;
            ug.rotacionando = 0;
            ug.escalando = 0;
            ug.cisalhando = 0;
            break;
        case SELECIONAR_PONTO:
            ug.adicionandoPonto = 0;
            ug.adicionandoReta = 0;
            ug.adicionandoPoligono = 0;
            ug.selecionandoPonto = 1;
            ug.selecionandoReta = 0;
            ug.selecionandoPoligono = 0;
            ug.transladando = 0;
            ug.rotacionando = 0;
            ug.escalando = 0;
            ug.cisalhando = 0;
            break;
        case SELECIONAR_RETA:
            ug.adicionandoPonto = 0;
            ug.adicionandoReta = 0;
            ug.adicionandoPoligono = 0;
            ug.selecionandoPonto = 0;
            ug.selecionandoReta = 1;
            ug.selecionandoPoligono = 0;
            ug.transladando = 0;
            ug.rotacionando = 0;
            ug.escalando = 0;
            ug.cisalhando = 0;
            break;
        case SELECIONAR_POLIGONO:
            ug.adicionandoPonto = 0;
            ug.adicionandoReta = 0;
            ug.adicionandoPoligono = 0;
            ug.selecionandoPonto = 0;
            ug.selecionandoReta = 0;
            ug.selecionandoPoligono = 1;
            ug.transladando = 0;
            ug.rotacionando = 0;
            ug.escalando = 0;
            ug.cisalhando = 0;
            break;
        case TRANSLADAR:
            break;
        case ROTACIONAR:
            break;
        case ESCALAR:
            break;
        case ESPELHAR_X:
            break;
        case ESPELHAR_Y:
            break;
        case CISALHAR:
            break;
        case DELETAR:
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
    glutAddMenuEntry("Shear", CISALHAR);
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