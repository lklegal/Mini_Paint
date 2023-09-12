#include <GL/glut.h>
#include <stdio.h>
#include "cabecalhos.h"

//tratamento de opções do menu
void menuPrincipalCallback(int opcao) {
    //guarda o estado anterior do sistema, caso seja necessário retornar a ele
    char estadoAnterior = ug.estado;
    //muda o estado do sistema para o escolhido no menu
    ug.estado = opcao;
    //criação e seleção de pontos, retas e polígonos
    if(opcao == CRIAR_PONTO || opcao == CRIAR_RETA || opcao == CRIAR_POLIGONO
    || opcao == SELECIONAR_PONTO || opcao == SELECIONAR_RETA || opcao == SELECIONAR_POLIGONO){
        //se algo foi selecionado antes, fica desselecionado (é apenas um bom momento para fazer isso)
        ug.escolhidos[0] = NULL;
        ug.escolhidos[1] = NULL;
        ug.objetoSelecionado = NULL;
        glutPostRedisplay();
        return;
    }
    /*se não for uma opção de criar nem de selecionar, significa que é uma opção que precisa ter
    alguém selecionado (transformação ou deletar). ug.ecolhidos[0] só vai ser NULL se ninguém foi
    selecionado.*/
    if(ug.escolhidos[0] == NULL){
        ug.estado = estadoAnterior;
        printf("Nada foi selecionado.\n");
        return;
    }
    /*ug.escolhidos[1] só vai ser NULL se um ponto foi selecionado. Mas um ponto só pode ser transladado,
    rotacionado ou escalado, portanto se foi escolhida outra opção ela é abortada.*/
    if((ug.escolhidos[1]) == NULL && opcao != TRANSLADAR && opcao != ROTACIONAR && opcao != DELETAR){
        ug.estado = estadoAnterior;
        printf("Não é possível fazer isso com pontos.\n");
        return;
    }
    /*ug.escolhidos[1] só vai ser NULL se um ponto foi selecionado. Mas se a opção escolhida foi
    rotacionar, e o que foi selecionado não foi um ponto solto, a opção escolhida é abortada,
    porque rotacionar o vértice de uma reta ou polígono em torno da origem seria bem estranho*/
    if((ug.escolhidos[1]) == NULL && opcao == ROTACIONAR && !(ug.objetoSelecionado)->ponto){
        ug.estado = estadoAnterior;
        printf("Só é possível rotacionar em relação à origem pontos soltos.\n");
        return;
    }
    //tratamento das opções de espelhamento
    if(opcao == ESPELHAR_X || opcao == ESPELHAR_Y){
        ug.estado = estadoAnterior;
        //a variável que define se o objeto será espelhado em x ou em y
        float eixoEspelhamento = 1.0;
        if(ug.estado == ESPELHAR_Y){
            eixoEspelhamento = -1.0;
        }

        /*A transformação foi feita separada das outras na função do mouse, porque aqui clicou,
        está pronto. Em outras palavras, espelhamento não depende da rolagem do mouse. Provavelmente
        dava pra fazer algo melhor do que repetir o código na cara de pau... mas funcionando é
        o que importa :D*/

        vertice centroide = calcularCentroide();

        float primeiraMatriz[3][3] = {
            {1.0, 0.0, centroide.x},
            {0.0, 1.0, centroide.y},
            {0.0, 0.0, 1.0}
        };

        float segundaMatriz[3][3] = {
			{-1.0*eixoEspelhamento, 0.0, 0.0},
			{0.0, 1.0*eixoEspelhamento, 0.0},
			{0.0, 0.0, 1.0}
		};

        float matrizIntermediaria[3][3];
        float terceiraMatriz[3][3] = {
            {1.0, 0.0, -centroide.x},
            {0.0, 1.0, -centroide.y},
            {0.0, 0.0, 1.0}
        };
        float matrizResultante[3][3];

        //multiplica as matrizes para achar a matriz de transformação
        MatMul(primeiraMatriz, segundaMatriz, matrizIntermediaria);
        MatMul(matrizIntermediaria, terceiraMatriz, matrizResultante);
        //multiplica a matriz de transformação pelos pontos do objeto
        trataMatVecMul(matrizResultante);
    }

    //deleta objetos selecionados
    if(opcao == DELETAR){
        ug.estado = estadoAnterior;
        //caso um vértice de reta ou polígono tenha sido selecionado, aborta
        if(!((ug.objetoSelecionado)->ponto) && ug.escolhidos[1] == NULL){
            ug.escolhidos[0] = NULL;
            ug.escolhidos[1] = NULL;
            ug.objetoSelecionado = NULL;
            glutPostRedisplay();
            printf("Apenas pontos isolados podem ser deletados.\n");
            return;
        }
        /*caso uma aresta de polígono tenha sido selecionada, aborta.
        ug.escolhidos[1] != ug.escolhidos[0] significa que é um vértice ou uma aresta,
        mas como o caso de vértices foi tratado acima, é uma aresta*/
        if((ug.objetoSelecionado)->poligono && ug.escolhidos[1] != ug.escolhidos[0]){
            ug.escolhidos[0] = NULL;
            ug.escolhidos[1] = NULL;
            ug.objetoSelecionado = NULL;
            glutPostRedisplay();
            printf("Apenas linhas isoladas podem ser deletadas.\n");
            return;
        }
        //desseleciona o que foi escolhido, chama a função de remover e atualiza a tela
        ug.escolhidos[0] = NULL;
        ug.escolhidos[1] = NULL;
        removerObjeto();
        glutPostRedisplay();
    }
}

//exibe menu com opções
void criarMenu() {
    glutCreateMenu(menuPrincipalCallback);
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
}

//O menu é desabilitado quando se está criando uma reta ou polígono
void HabilitarDesabilitarMenu(){
    if(ug.podeExibirMenu){
        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }else{
        glutDetachMenu(GLUT_RIGHT_BUTTON);
    }
}