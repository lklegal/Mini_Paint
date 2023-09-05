typedef struct VerticeObjeto vertice;
typedef struct Objeto obj;

obj** criarListaDeObjetos();
vertice** criarListaDeVerticesDoObjeto();
void adicionarObjeto(obj **listaDeObjetos);
void adicionarVertice(obj **listaDeObjetos, float x, float y);
void removerObjeto(obj **listaDeObjetos, int posicao);
int init();
void display();
float** MatMul(float a[3][3], float b[3][3]);
float* MatVecMul(float a[3][3], float *b);