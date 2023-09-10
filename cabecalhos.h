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

struct Objeto{
	struct VerticeObjeto **vertices;
	struct Objeto *prox;
    char ponto;
    char reta;
    char poligono;
    char incompleto;
};

struct VerticeObjeto{
    float x;
    float y;
	struct VerticeObjeto *prox;
};

typedef struct VerticeObjeto vertice;
typedef struct Objeto obj;

//será usado como uma variável global para permitir a comunicação entre funções callback e o restante
struct UtilidadesGlobais
{
    char estado;
	obj **listaDeObjetos;
    obj *objetoSendoCriado;
    vertice** escolhidos;
    char transladando;
    char podeExibirMenu;
};

extern struct UtilidadesGlobais ug;

obj** criarListaDeObjetos();
vertice** criarListaDeVerticesDoObjeto();
obj* adicionarObjeto(obj **listaDeObjetos);
void adicionarVertice(obj **listaDeObjetos, obj *objetoEspecifico, float x, float y);
void removerObjeto(obj **listaDeObjetos, int posicao);
int init();
void display();
float** MatMul(float a[3][3], float b[3][3]);
float* MatVecMul(float a[3][3], float *b);
void criarMenu();
void HabilitarDesabilitarMenu();
vertice calcularCentroide();