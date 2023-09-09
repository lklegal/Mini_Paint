struct Objeto{
	struct VerticeObjeto **vertices;
	struct Objeto *prox;
    char ponto;
    char reta;
    char poligono;
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
    char adicionandoPonto;
    char adicionandoReta;
    char adicionandoPoligono;
	obj **listaDeObjetos;
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