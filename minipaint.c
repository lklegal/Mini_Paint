#include <stdio.h>
#include <stdlib.h>
#include "cabecalhos.h"
#include <GL/glut.h>

struct UtilidadesGlobais ug;

void adicionarPonto(int x, int y){
	obj *novoObjetoPonto = adicionarObjeto(ug.listaDeObjetos);
	adicionarVertice(ug.listaDeObjetos, novoObjetoPonto, ((float)x)/4.0, (600.0-(float)y)/4.0);
	novoObjetoPonto->incompleto = 0;
	glutPostRedisplay();
}

void adicionarReta(int x, int y){
	if(ug.objetoSendoCriado == NULL){
		ug.objetoSendoCriado = adicionarObjeto(ug.listaDeObjetos);
		ug.podeExibirMenu = 0;
		adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600.0-(float)y)/4.0);
	}else{
		adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600.0-(float)y)/4.0);
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
		adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600.0-(float)y)/4.0);
	}else{
		if(ultimoVertice){
			ug.objetoSendoCriado->incompleto = 0;
			ug.podeExibirMenu = 1;
			ug.objetoSendoCriado = NULL;
		}else{
			adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600.0-(float)y)/4.0);
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

void codificarReta(char vetorCod[4], float px, float py, float mx, float my, float tolerancia){
	vetorCod[0] = px < mx - tolerancia;
	vetorCod[1] = px > mx + tolerancia;
	vetorCod[2] = py < my - tolerancia;
	vetorCod[3] = py > my + tolerancia;
}

char casosTriviaisReta(char codV1[4], char codV2[4]){
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

	codificarReta(codV2, v2->x, v2->y, mx, my, tolerancia);
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
		codificarReta(codV1, x_cabinha, y_cabinha, mx, my, tolerancia);
		solucao = casosTriviaisReta(codV1, codV2);
	} while(solucao == 2);
	
	return solucao;
}

char pickPoligono(obj *poligono, float mx, float my){
	int contador = 0;
	float abscissaIntersecao = 0.0;
	/*Aqui é preciso uma circularidade e acesso ao anterior, mas listas circulares duplamente
	encadeadas teriam um peso desnecessário, então resolvi tratar aqui, no único caso em que
	isso é necessário. Como o anterior do primero é o último, e eu não tenho referência a ele,
	o algoritmo começa a partir do segundo e termina no primeiro. E como é um polígono, eu sei
	que ele tem pelo menos 3 vértices.*/
	vertice *aux = (*(poligono->vertices))->prox;
	vertice *ant = *(poligono->vertices);
	vertice *prox = aux->prox;
	//controle, para saber quando o laço termina
	vertice *primeiro = NULL;

	while(primeiro != aux){
		if(primeiro == NULL){
			primeiro = aux;
		}

		if((aux->y > my && prox->y > my) || (aux->y < my && prox->y < my) || (aux->x < mx && prox->x < mx) 
		|| aux->y == prox->y){
			contador = contador;
		}else if(((aux->x > mx && prox->x > mx) && ((aux->y > my && prox->y < my) || 
		(aux->y < my && prox->y > my)))){
			contador++;
		}else if(aux->y != my && !(aux->y > my && prox->y > my) && !(aux->y < my && prox->y < my)
		&& !(aux->x < mx && prox->x < mx) && aux->y != prox->y){
			abscissaIntersecao = aux->x + (my - aux->y) * ((prox->x - aux->x) / (prox->y - aux->y));
			if(abscissaIntersecao > mx){
				contador++;
			}
		}else if(aux->x > mx){
			if(prox->y < my){
				contador++;
			}
			if(ant->y < my){
				contador++;
			}
		}

		ant = aux;
		aux = prox;
		if(prox->prox == NULL){
			//volta para o início se for o último
			prox = *(poligono->vertices);
		}else{
			//caso contrário, vai para o próximo
			prox = prox->prox;
		}
	}
	if(contador%2 == 0){
		return 0;
	}else{
		return 1;
	}
}

void escolherAlgo(int mx, int my){
	obj *aux = *(ug.listaDeObjetos);
	vertice *aux2;
	char escolheu = 0;
	while(aux != NULL){
		aux2 = *(aux->vertices);
		while(aux2 != NULL){
			if(ug.estado == SELECIONAR_PONTO){
				escolheu = pickPonto(aux2->x, aux2->y, ((float)mx)/4.0, (600.0-(float)my)/4.0, 2.0);
				if(escolheu){
					printf("Escolheu o ponto (%.1f, %.1f)\n", aux2->x, aux2->y);
					ug.escolhidos[0] = aux2;
					ug.escolhidos[1] = NULL;
					ug.objetoSelecionado = aux;
					return;
				}
			}
			if(ug.estado == SELECIONAR_RETA){
				if(aux2->prox != NULL){
					escolheu = pickReta(aux2, aux2->prox, ((float)mx)/4.0, (600.0-(float)my)/4.0, 2.0);
				}else if(aux->poligono){
					escolheu = pickReta(aux2, *(aux->vertices), ((float)mx)/4.0, (600.0-(float)my)/4.0, 2.0);
				}
				if(escolheu){
					if(aux2->prox != NULL){
						printf("Escolheu a reta de vértices (%.1f, %.1f), (%.1f, %.1f)\n", aux2->x, aux2->y,
						aux2->prox->x, aux2->prox->y);
						ug.escolhidos[0] = aux2;
						ug.escolhidos[1] = aux2->prox;
					}else if(aux->poligono){
						printf("Escolheu a reta de vértices (%.1f, %.1f), (%.1f, %.1f)\n", aux2->x, aux2->y,
						(*(aux->vertices))->x, (*(aux->vertices))->y);
						ug.escolhidos[0] = aux2;
						ug.escolhidos[1] = *(aux->vertices);
					}
					ug.objetoSelecionado = aux;
					return;
				}
			}
			if(ug.estado == SELECIONAR_POLIGONO){
				if(!aux->poligono){
					break;
				}
				escolheu = pickPoligono(aux, ((float)mx)/4.0, (600.0-(float)my)/4.0);
				if(escolheu){
					aux2 = *(aux->vertices);
					printf("Escolheu o polígono de vértices ");
					while(aux2 != NULL){
						printf("(%.1f, %.1f) ", aux2->x, aux2->y);
						aux2 = aux2->prox;
					}
					printf("\n");
					ug.escolhidos[0] = *(aux->vertices);
					//gambiarra pra saber se é um ponto selecionado ou um polígono
					ug.escolhidos[1] = *(aux->vertices);
					ug.objetoSelecionado = aux;
					return;
				}else{
					//Para polígonos é melhor fazer o laço interno na função pickPoligono
					break;
				}
			}
			aux2 = aux2->prox;
		}
		aux = aux->prox;
	}
}

void casosMatVecMul(float matrizTransformacao[3][3]){
	float vetorPonto[3] = {0, 0, 1};
	float *resultado;
	if(ug.escolhidos[1] == NULL){
		vetorPonto[0] = (ug.escolhidos[0])->x;
		vetorPonto[1] = (ug.escolhidos[0])->y;
		resultado = MatVecMul(matrizTransformacao, vetorPonto);
		(ug.escolhidos[0])->x = resultado[0];
		(ug.escolhidos[0])->y = resultado[1];
	}else if(ug.escolhidos[0] != ug.escolhidos[1]){
		vetorPonto[0] = (ug.escolhidos[0])->x;
		vetorPonto[1] = (ug.escolhidos[0])->y;
		resultado = MatVecMul(matrizTransformacao, vetorPonto);
		(ug.escolhidos[0])->x = resultado[0];
		(ug.escolhidos[0])->y = resultado[1];
		vetorPonto[0] = (ug.escolhidos[1])->x;
		vetorPonto[1] = (ug.escolhidos[1])->y;
		resultado = MatVecMul(matrizTransformacao, vetorPonto);
		(ug.escolhidos[1])->x = resultado[0];
		(ug.escolhidos[1])->y = resultado[1];
	}else{
		vertice *aux = ug.escolhidos[0];
		while(aux != NULL){
			vetorPonto[0] = aux->x;
			vetorPonto[1] = aux->y;
			resultado = MatVecMul(matrizTransformacao, vetorPonto);
			aux->x = resultado[0];
			aux->y = resultado[1];
			aux = aux->prox;
		}
	}
	free(resultado);
	glutPostRedisplay();
}

//implementação da translação em ponto, reta e polígono
void movimentoMouse(int x, int y) {
    if(ug.transladando){
		vertice centroide = calcularCentroide();
		float mx = (float)x/4.0;
		float my = (600.0-(float)y)/4.0;
		float matrizTranslacao[3][3] ={
			{1.0, 0.0, mx - centroide.x},
			{0.0, 1.0, my - centroide.y},
			{0.0, 0.0, 1.0}
		};
		casosMatVecMul(matrizTranslacao);
	}
}

void mouse(int botao, int clicouOuSoltou, int x, int y) {
    if (botao == GLUT_LEFT_BUTTON && clicouOuSoltou == GLUT_DOWN) {
        //printf("Clique do mouse em (%d, %d)\n", x, y);
		if(ug.estado == CRIAR_PONTO){
			adicionarPonto(x, y);
		}else if(ug.estado == CRIAR_RETA){
			adicionarReta(x, y);
		}else if(ug.estado == CRIAR_POLIGONO){
			adicionarPoligono(x, y, 0);
		}else if(ug.estado == SELECIONAR_PONTO || ug.estado == SELECIONAR_RETA || ug.estado == SELECIONAR_POLIGONO){
			escolherAlgo(x, y);
		}else if(ug.estado == TRANSLADAR){
			ug.transladando = 1;
		}
    }
	if (botao == GLUT_LEFT_BUTTON && clicouOuSoltou == GLUT_UP) {
		if(ug.transladando){
			ug.transladando = 0;
		}
        //printf("Botão do mouse solto em (%d, %d)\n", x, y);
    }
	if (botao == GLUT_RIGHT_BUTTON && clicouOuSoltou == GLUT_DOWN) {
        if(ug.estado == CRIAR_POLIGONO){
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

	if(!(ug.estado == ROTACIONAR || ug.estado == ESCALAR || ug.estado == CISALHAR_X || ug.estado == CISALHAR_Y)){
		return;
	}

	int cimaOuBaixo = 0;

	if(botao == 3){
		cimaOuBaixo = 1;
	}
	if(botao == 4){
		cimaOuBaixo = -1;
	}

	if(cimaOuBaixo != 0){
		//rotaciona em incrementos de 1 grau
		float senoIncremento = -0.017452 * cimaOuBaixo;
		float cossenoIncremento = 0.999847;

		float fatorCisalhamento = 0.1 * cimaOuBaixo;
		float fatorEscala;
		if(cimaOuBaixo == 1){
			fatorEscala = 1.1;
		}else{
			fatorEscala = 1/1.1;
		}

		float eixoEspelhamento = 1.0;
		if(ug.estado == ESPELHAR_Y){
            eixoEspelhamento = -1.0;
        }
		vertice centroide = calcularCentroide();
		float primeiraMatriz[3][3] = {
			{1.0, 0.0, centroide.x},
			{0.0, 1.0, centroide.y},
			{0.0, 0.0, 1.0}
		};
		float segundaMatriz[3][3];
		float matrizIntermediaria[3][3];
		float terceiraMatriz[3][3] = {
			{1.0, 0.0, -centroide.x},
			{0.0, 1.0, -centroide.y},
			{0.0, 0.0, 1.0}
		};
		float matrizResultante[3][3];
		if(ug.estado == ROTACIONAR){
			segundaMatriz[0][0] = cossenoIncremento;
			segundaMatriz[0][1] = -senoIncremento;
			segundaMatriz[0][2] = 0.0;
			segundaMatriz[1][0] = senoIncremento;
			segundaMatriz[1][1] = cossenoIncremento;
			segundaMatriz[1][2] = 0.0;
			segundaMatriz[2][0] = 0.0;
			segundaMatriz[2][1] = 0.0;
			segundaMatriz[2][2] = 1.0;
		}
		if(ug.estado == ESCALAR){
			segundaMatriz[0][0] = fatorEscala;
			segundaMatriz[0][1] = 0.0;
			segundaMatriz[0][2] = 0.0;
			segundaMatriz[1][0] = 0.0;
			segundaMatriz[1][1] = fatorEscala;
			segundaMatriz[1][2] = 0.0;
			segundaMatriz[2][0] = 0.0;
			segundaMatriz[2][1] = 0.0;
			segundaMatriz[2][2] = 1.0;
		}
		if(ug.estado == CISALHAR_X){
			segundaMatriz[0][0] = 1.0;
			segundaMatriz[0][1] = fatorCisalhamento;
			segundaMatriz[0][2] = 0.0;
			segundaMatriz[1][0] = 0.0;
			segundaMatriz[1][1] = 1.0;
			segundaMatriz[1][2] = 0.0;
			segundaMatriz[2][0] = 0.0;
			segundaMatriz[2][1] = 0.0;
			segundaMatriz[2][2] = 1.0;
		}
		if(ug.estado == CISALHAR_Y){
			segundaMatriz[0][0] = 1.0;
			segundaMatriz[0][1] = 0.0;
			segundaMatriz[0][2] = 0.0;
			segundaMatriz[1][0] = fatorCisalhamento;
			segundaMatriz[1][1] = 1.0;
			segundaMatriz[1][2] = 0.0;
			segundaMatriz[2][0] = 0.0;
			segundaMatriz[2][1] = 0.0;
			segundaMatriz[2][2] = 1.0;
		}

		MatMul(primeiraMatriz, segundaMatriz, matrizIntermediaria);
		MatMul(matrizIntermediaria, terceiraMatriz, matrizResultante);
		casosMatVecMul(matrizResultante);
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
	glutMouseFunc(mouse);
	glutMotionFunc(movimentoMouse);
}