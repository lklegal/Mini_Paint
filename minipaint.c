#include <stdio.h>
#include <stdlib.h>
#include "cabecalhos.h"
#include <GL/glut.h>

/*Vai ser comum de ver pontos x e y serem tratados dessa forma:
(float)x/4.0, (600.0-(float)y)/4.0
Isso é a conversão das coordenadas do mouse na tela para as coordenadas da janela de recorte.
Os valores específicos têm a ver com o tamanho escolhido para a tela, 800x600*/

struct UtilidadesGlobais ug;

void adicionarPonto(int x, int y){
	obj *novoObjetoPonto = adicionarObjeto(ug.listaDeObjetos);
	adicionarVertice(ug.listaDeObjetos, novoObjetoPonto, ((float)x)/4.0, (600.0-(float)y)/4.0);
	//pontos são simples e já são criados completos
	novoObjetoPonto->incompleto = 0;
	glutPostRedisplay();
}

void adicionarReta(int x, int y){
	/*se objetoSendoCriado ainda for NULL, significa que esse é o primeiro vértice sendo criado
	e a reta sairá dessa função incompleta, apenas com 1 vértice*/
	if(ug.objetoSendoCriado == NULL){
		ug.objetoSendoCriado = adicionarObjeto(ug.listaDeObjetos);
		adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600.0-(float)y)/4.0);
		ug.podeExibirMenu = 0;
	}else{
		adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600.0-(float)y)/4.0);
		ug.objetoSendoCriado->incompleto = 0;
		ug.podeExibirMenu = 1;
		ug.objetoSendoCriado = NULL;
	}
	HabilitarDesabilitarMenu();
	glutPostRedisplay();
}

//Adiciona polígono. ultimoVertice, como o nome sugere, diz se é o último vértice do polígono ou não
void adicionarPoligono(int x, int y, char ultimoVertice){
	//se for o primeiro vértice
	if(ug.objetoSendoCriado == NULL){
		ug.podeExibirMenu = 0;
		ug.objetoSendoCriado = adicionarObjeto(ug.listaDeObjetos);
		adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600.0-(float)y)/4.0);
	}else{
		//se o último vértice já fo adicionado
		if(ultimoVertice){
			ug.podeExibirMenu = 1;
			ug.objetoSendoCriado->incompleto = 0;
			ug.objetoSendoCriado = NULL;
		}
		//se já adicionou algum vértice e quer adicionar mais
		else{
			adicionarVertice(ug.listaDeObjetos, ug.objetoSendoCriado, ((float)x)/4.0, (600.0-(float)y)/4.0);
		}
	}
	HabilitarDesabilitarMenu();
	glutPostRedisplay();
}

//algoritmo de seleção de ponto
char pickPonto(float px, float py, float mx, float my, float tolerancia){
	if(mx <= px+tolerancia && mx >= px-tolerancia){
		if(my <= py+tolerancia && my >= py-tolerancia){
			return 1;
		}
	}
	return 0;
}

//codifica a reta no esquema X X X X
void codificarReta(char vetorCod[4], float px, float py, float mx, float my, float tolerancia){
	vetorCod[0] = px < mx - tolerancia;
	vetorCod[1] = px > mx + tolerancia;
	vetorCod[2] = py < my - tolerancia;
	vetorCod[3] = py > my + tolerancia;
}


//checa os casos triviais da seleção de reta
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
	//vetores de codificação do primeiro vértice e do segundo
	char codV1[4] = {0, 0, 0, 0};
	char codV2[4] = {0, 0, 0, 0};

	codificarReta(codV2, v2->x, v2->y, mx, my, tolerancia);
	//para solução: 1 está dentro, 0 está fora, 2 indefinido
	char solucao = 2;

	float xCabinhaQueCorre = v1->x;
	float yCabinhaQueCorre = v1->y;

	do
	{
		/*na primeira iteração não cairá em nenhuma das 4 condições, já que codV1 ainda não
		foi codificado, portanto ainda está com sua codificação padrão de 0000*/
		if(codV1[0]){//se estiver à esquerda, vai para a direita
			xCabinhaQueCorre = mx - tolerancia;
			yCabinhaQueCorre = ((v2->y - v1->y) / (v2->x - v1->x)) * (xCabinhaQueCorre - v1->x) + v1->y;
		}else if(codV1[1]){//se estiver à direita, vai para a esquerda
			xCabinhaQueCorre = mx + tolerancia;
			yCabinhaQueCorre = ((v2->y - v1->y) / (v2->x - v1->x)) * (xCabinhaQueCorre - v1->x) + v1->y;
		}else if(codV1[2]){//se estiver abaixo, vai para cima
			yCabinhaQueCorre = my - tolerancia;
			xCabinhaQueCorre = ((v2->x - v1->x) / (v2->y - v1->y)) * (yCabinhaQueCorre - v1->y) + v1->x;
		}else if(codV1[3]){//se estiver acima, vai para baixo
			yCabinhaQueCorre = my + tolerancia;
			xCabinhaQueCorre = ((v2->x - v1->x) / (v2->y - v1->y)) * (yCabinhaQueCorre - v1->y) + v1->x;
		}

		/*Codificando com base na posição do cabinha que corre. Na primeira iteração codificará
		o primeiro vértice da reta, que será igual ao cabinha que corre*/
		codificarReta(codV1, xCabinhaQueCorre, yCabinhaQueCorre, mx, my, tolerancia);
		//checa casos triviais
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

	//segundo vértice, primeiro a ser analisado
	vertice *aux = (*(poligono->vertices))->prox;
	//primeiro vértice
	vertice *ant = *(poligono->vertices);
	//terceiro vértice
	vertice *prox = aux->prox;
	//controle, para saber quando o laço termina
	vertice *primeiro = NULL;

	while(primeiro != aux){
		if(primeiro == NULL){
			primeiro = aux;
		}

		//casos onde descarta a reta
		if((aux->y > my && prox->y > my) || (aux->y < my && prox->y < my) || (aux->x < mx && prox->x < mx) 
		|| aux->y == prox->y){
			contador = contador;
		}
		//caso onde a reta está à direita do tiro, com um vértice acima e outro abaixo
		else if(((aux->x > mx && prox->x > mx) && ((aux->y > my && prox->y < my) || 
		(aux->y < my && prox->y > my)))){
			contador++;
		}
		//caso onde a reta não está nem totalmente à direita, à esquerda, acima ou abaixo
		else if(aux->y != my && !(aux->y > my && prox->y > my) && !(aux->y < my && prox->y < my)
		&& !(aux->x < mx && prox->x < mx) && aux->y != prox->y){
			abscissaIntersecao = aux->x + (my - aux->y) * ((prox->x - aux->x) / (prox->y - aux->y));
			if(abscissaIntersecao > mx){
				contador++;
			}
		}
		/*Caso ultra raro do tiro intersectar com um vértice à direita. Esse caso é o que torna
		necessário um anterior: é preciso analisar os dois vértices adjacentes a ele*/
		else if(aux->x > mx){
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

	//checa a paridade das colisões do tiro para saber se o clique foi dentro ou fora do polígono
	if(contador%2 == 0){
		return 0;
	}else{
		return 1;
	}
}

//controla as funções de seleção
void escolherAlgo(int mx, int my){
	obj *aux = *(ug.listaDeObjetos);
	vertice *aux2;
	char escolheu = 0;
	//percorre cada objeto para ver se ele ou algo dele foi selecionado
	while(aux != NULL){
		aux2 = *(aux->vertices);
		//percorre cada vértice do objeto
		while(aux2 != NULL){
			//trata seleção de ponto
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
			//trata seleção de reta
			if(ug.estado == SELECIONAR_RETA){
				vertice *v2Aresta;
				/*se não estiver no fim da lista de vértices, pega o vértice atual e o próximo e verifica 
				se escolheu a reta definida por eles*/
				if(aux2->prox != NULL){
					v2Aresta = aux2->prox;
					escolheu = pickReta(aux2, v2Aresta, ((float)mx)/4.0, (600.0-(float)my)/4.0, 2.0);
				/*se estiver no fim da lista de vértices, mas o objeto analisado for um polígono.
				Possibilita a seleção da reta definida pelo último vértice do polígono e o primeiro*/
				}else if(aux->poligono){
					//aux2 é o último vértice do polígono, *(aux->vertices) é o primeiro
					v2Aresta = *(aux->vertices);
					escolheu = pickReta(aux2, v2Aresta, ((float)mx)/4.0, (600.0-(float)my)/4.0, 2.0);
				}
				if(escolheu){
					printf("Escolheu a reta de vértices (%.1f, %.1f), (%.1f, %.1f)\n", aux2->x, aux2->y,
					v2Aresta->x, v2Aresta->y);
					ug.escolhidos[0] = aux2;
					ug.escolhidos[1] = aux2->prox;
					ug.objetoSelecionado = aux;
					return;
				}
			}
			//trata seleção de polígono
			if(ug.estado == SELECIONAR_POLIGONO){
				//se o objeto analisado não for um polígono, sai do laço interno e vai para o próximo
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

//lida com multiplicações de matriz por vetor (ponto)
void trataMatVecMul(float matrizTransformacao[3][3]){
	//vetor homogêneo
	float vetorPonto[3] = {0, 0, 1};
	float *resultado;
	//caso seja um vértice
	if(ug.escolhidos[1] == NULL){
		vetorPonto[0] = (ug.escolhidos[0])->x;
		vetorPonto[1] = (ug.escolhidos[0])->y;
		resultado = MatVecMul(matrizTransformacao, vetorPonto);
		(ug.escolhidos[0])->x = resultado[0];
		(ug.escolhidos[0])->y = resultado[1];
	}
	//caso seja uma aresta
	else if(ug.escolhidos[0] != ug.escolhidos[1]){
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
	}
	//caso seja um polígono
	else{
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
	//como resultado guarda alguém alocado dinamicamente na função MatVecMul, precisa ser liberado
	free(resultado);
	//atualiza a tela
	glutPostRedisplay();
	/*
	//vetor homogêneo
	float vetorPonto[3] = {0, 0, 1};
	float *resultado;
	vertice *aux = *((ug.objetoSelecionado)->vertices);
	//aplica a transformação em cada vértice do objeto
	while(aux != NULL){
		vetorPonto[0] = aux->x;
		vetorPonto[1] = aux->y;
		resultado = MatVecMul(matrizTransformacao, vetorPonto);
		aux->x = resultado[0];
		aux->y = resultado[1];
		aux = aux->prox;
	}
	//como resultado guarda alguém alocado dinamicamente na função MatVecMul, precisa ser liberado
	free(resultado);
	//atualiza a tela
	glutPostRedisplay();
	*/
}

//implementação da translação em ponto, reta e polígono
void movimentoMouse(int x, int y) {
    if(ug.transladando){
		vertice centroide = calcularCentroide();
		float mx = (float)x/4.0;
		float my = (600.0-(float)y)/4.0;
		//os incrementos em x e y são a diferença entre a posição do mouse e a do centroide
		float matrizTranslacao[3][3] ={
			{1.0, 0.0, mx - centroide.x},
			{0.0, 1.0, my - centroide.y},
			{0.0, 0.0, 1.0}
		};
		//multiplica a matriz de translação pelos pontos do objeto
		trataMatVecMul(matrizTranslacao);
	}
}


//função enorme que lida com tudo o que dá pra fazer com o mouse
void mouse(int botao, int clicouOuSoltou, int x, int y) {
	//caso o botão esquerdo seja pressionado
    if (botao == GLUT_LEFT_BUTTON && clicouOuSoltou == GLUT_DOWN) {
		//cria pontos, retas e polígonos
		if(ug.estado == CRIAR_PONTO){
			adicionarPonto(x, y);
		}else if(ug.estado == CRIAR_RETA){
			adicionarReta(x, y);
		}else if(ug.estado == CRIAR_POLIGONO){
			adicionarPoligono(x, y, 0);
		//chama a função que controla os algoritmos de seleção
		}else if(ug.estado == SELECIONAR_PONTO || ug.estado == SELECIONAR_RETA || ug.estado == SELECIONAR_POLIGONO){
			escolherAlgo(x, y);
			if(ug.objetoSelecionado != NULL){
				glutPostRedisplay();
			}
		//Faz o programa saber que algo está sendo transladado
		}else if(ug.estado == TRANSLADAR){
			ug.transladando = 1;
		}
    }
	//caso o botão esquerdo seja solto
	if (botao == GLUT_LEFT_BUTTON && clicouOuSoltou == GLUT_UP) {
		//se estiver arrastando alguém, para
		if(ug.transladando){
			ug.transladando = 0;
		}
    }
	//trata o clique do botão direito caso um polígono esteja sendo criado
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

	//se houve uma interação com o mouse mas o estado do programa não é de uma transformação, acaba aqui
	if(!(ug.estado == ROTACIONAR || ug.estado == ESCALAR || ug.estado == CISALHAR_X || ug.estado == CISALHAR_Y)){
		return;
	}
	//verifica se a rolagem do mouse foi para cima ou para baixo
	int cimaOuBaixo = 0;

	if(botao == 3){
		cimaOuBaixo = 1;
	}
	if(botao == 4){
		cimaOuBaixo = -1;
	}
	//se o evento de mouse que iniciou essa função foi uma rolagem
	if(cimaOuBaixo != 0){
		//rotações são feitas em incrementos de +-1 grau, dependendo da rolagem
		float senoIncremento = -0.017452 * cimaOuBaixo;
		float cossenoIncremento = 0.999847;
		//cisalhamento positivo ou negativo dependendo da rolagemss
		float fatorCisalhamento = 0.1 * cimaOuBaixo;
		float fatorEscala;
		//aumenta ou diminui o objeto de acordo com a rolagem
		if(cimaOuBaixo == 1){
			fatorEscala = 1.1;
		}else{
			fatorEscala = 1/1.1;
		}
		//calcul o centroide do objeto a ser transformado
		vertice centroide = calcularCentroide();
		//matriz de translação para a origem
		float primeiraMatriz[3][3] = {
			{1.0, 0.0, centroide.x},
			{0.0, 1.0, centroide.y},
			{0.0, 0.0, 1.0}
		};
		//matriz da transformação específica
		float segundaMatriz[3][3] = {
			{1.0, 0.0, 0.0},
			{0.0, 1.0, 0.0},
			{0.0, 0.0, 1.0}
		};
		//guarda o resultado da primeira multiplicação
		float matrizIntermediaria[3][3];

		//matriz de translação de volta da origem
		float terceiraMatriz[3][3] = {
			{1.0, 0.0, -centroide.x},
			{0.0, 1.0, -centroide.y},
			{0.0, 0.0, 1.0}
		};
		//guarda a matriz de transformação a ser aplicada em cada vértice do objeto
		float matrizResultante[3][3];
		//seta as matrizes de cada transformação
		if(ug.estado == ROTACIONAR){
			segundaMatriz[0][0] = cossenoIncremento;
			segundaMatriz[0][1] = -senoIncremento;
			segundaMatriz[1][0] = senoIncremento;
			segundaMatriz[1][1] = cossenoIncremento;
		}
		if(ug.estado == ESCALAR){
			segundaMatriz[0][0] = fatorEscala;
			segundaMatriz[1][1] = fatorEscala;
		}
		if(ug.estado == CISALHAR_X){
			segundaMatriz[0][1] = fatorCisalhamento;
		}
		if(ug.estado == CISALHAR_Y){
			segundaMatriz[1][0] = fatorCisalhamento;
		}
		//multiplica as matrizes, multiplica a matriz resultante por cada ponto do objeto
		MatMul(primeiraMatriz, segundaMatriz, matrizIntermediaria);
		MatMul(matrizIntermediaria, terceiraMatriz, matrizResultante);
		trataMatVecMul(matrizResultante);
	}
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	//percorre cada objeto
	obj *aux = *(ug.listaDeObjetos);
	//percorre cada vértice de cada objeto
	vertice *aux2;
	while(aux != NULL){
		if(aux == ug.objetoSelecionado){
			//redão quando selecionado
			glColor3f(1.0, 0.0, 0.0);
			/*basicamente só pinta tudo de vermelho se o que for selecionado for ponto solto, reta solta
			ou polígono inteiro, caso contrário destaca so o vértice ou aresta selecionado*/
			if(((ug.objetoSelecionado)->poligono && ug.escolhidos[1] != ug.escolhidos[0])
			|| ((ug.objetoSelecionado)->reta && ug.escolhidos[1] == NULL)){
				glBegin(GL_POINTS);
					glVertex2f((ug.escolhidos[0])->x, (ug.escolhidos[0])->y);
				glEnd();
				if(ug.escolhidos[1] != NULL){
					glBegin(GL_POINTS);
						glVertex2f((ug.escolhidos[1])->x, (ug.escolhidos[1])->y);
					glEnd();
				}
				glColor3f(0.0, 0.0, 0.0);	
			}
		}else{
			//blackão caso contrário
			glColor3f(0.0, 0.0, 0.0);
		}
		aux2 = *(aux->vertices);
		//renderiza pontos
		if(aux->ponto){
			glBegin(GL_POINTS);
				while(aux2 != NULL){
					glVertex2f(aux2->x, aux2->y);
					aux2 = aux2->prox;
				}
			glEnd();
		//renderiza retas
		}else if(aux->reta){
			//caso ainda só tenha sido criado o primeiro vértice, exibe ele como ponto
			if(aux->incompleto){
				glBegin(GL_POINTS);
					glVertex2f(aux2->x, aux2->y);
				glEnd();
			//caso contrário, exibe a reta com os dois pontos criados
			}else{
				glBegin(GL_LINES);
					while(aux2 != NULL){
						glVertex2f(aux2->x, aux2->y);
						aux2 = aux2->prox;
					}
				glEnd();
			}
		//renderiza polígonos
		}else{
			//caso ainda só tenha sido criado o primeiro vértice, exibe ele como ponto
			if(aux->incompleto && aux2->prox == NULL){
				glBegin(GL_POINTS);
					glVertex2f(aux2->x, aux2->y);
				glEnd();
			/*caso tenha mais de um vértice mas ainda esteja criando, exibe retas para cada par de
			vértices adjacentes*/
			}else if(aux->incompleto && aux2->prox != NULL){
				while(aux2->prox != NULL){
					glBegin(GL_LINES);
						glVertex2f(aux2->x, aux2->y);
						glVertex2f(aux2->prox->x, aux2->prox->y);
					glEnd();
					aux2 = aux2->prox;
				}
			//caso contrário, exibe como polígono
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
	//troca os frame buffers
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