#define CRIAR_PONTO 1
#define CRIAR_RETA 2
#define CRIAR_POLIGONO 3
#define SELECIONAR_PONTO 4
#define SELECIONAR_RETA 5
#define SELECIONAR_POLIGONO 6
#define TRANSLADAR 7
#define ROTACIONAR 8
#define ESCALAR 9
#define ESPELHAR_X 10
#define ESPELHAR_Y 11
#define CISALHAR_X 12
#define CISALHAR_Y 13
#define DELETAR 14

//objeto que contém os vértices: pode ser ponto, reta ou polígono
struct Objeto{
    //ponteiro para a lista interna de vértices
	struct VerticeObjeto **vertices;
	struct Objeto *prox;
    char ponto;
    char reta;
    char poligono;
    /*no caso de retas e polígonos que não foram finalizados, útil para
    definir a função do botão direito do mouse*/
    char incompleto;
    //id do objeto, útil na hora de deletar ele
    int id;
};

//vértice
struct VerticeObjeto{
    float x;
    float y;
	struct VerticeObjeto *prox;
};

typedef struct VerticeObjeto vertice;
typedef struct Objeto obj;

//usado como uma variável global para facilitar a comunicação entre funções, principalmente as callback
struct UtilidadesGlobais
{
    //se está criando, selecionando, transformando, deletando
    char estado;
    //ponteiro para a lista externa de objetos, cada um com seus vértices
    obj **listaDeObjetos;
    /*útil na hora de criar retas e polígonos, já que algumas funções são chamadas mais de uma vez
    separadamente, e precisam "lembrar" o endereço do objeto que estão criando*/
    obj *objetoSendoCriado;
    //referência ao objeto (ponto, linha ou polígono) selecionado
    obj *objetoSelecionado;
    /*Guarda um (ou dois) vértices do objeto selecionado, e dependendo da forma como os ponteiros
    ficam dá pra dizer se o que foi selecionado foi um vértice, aresta ou polígono. Meio gambiarra,
    mas é diferente do objeto selecionado. Por exemplo, eu posso selecionar um vértice de uma reta,
    ou a aresta de um polígono*/
    vertice** escolhidos;
    //informa se estou arrastando algum objeto com o mouse
    char transladando;
    //controle para a função HabilitarDesabilitarMenu
    char podeExibirMenu;
};

extern struct UtilidadesGlobais ug;

obj** criarListaDeObjetos();
vertice** criarListaDeVerticesDoObjeto();
obj* adicionarObjeto(obj **listaDeObjetos);
void adicionarVertice(obj **listaDeObjetos, obj *objetoEspecifico, float x, float y);
void removerObjeto();
int init();
void display();
void MatMul(float a[3][3], float b[3][3], float resultado[3][3]);
float* MatVecMul(float a[3][3], float *b);
void criarMenu();
void HabilitarDesabilitarMenu();
vertice calcularCentroide();
void trataMatVecMul(float matrizTransformacao[3][3]);