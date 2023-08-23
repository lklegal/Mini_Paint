#include <stdio.h>
#include <stdlib.h>

//retorna o resultado da multiplicação de duas matrizes 3x3
float** MatMul(float a[3][3], float b[3][3]){
    //cria a matriz resultante
    float **resultado = (float**)malloc(3*sizeof(float));
    for(int i = 0; i < 3; i++){
        resultado[i] = (float*)malloc(3*sizeof(float));
    }

    float soma = 0.0;

    //multiplica as matrizes
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            soma = 0.0;
            for(int k = 0; k < 3; k++){
                soma += a[i][k] * b[k][j];
            }
            //printf("%.1f ", soma);
            resultado[i][j] = soma;
        }
    }
    return resultado;
}

int main(){
    float a[3][3] = {
        {1.1, 3.5, 3.7},
        {4.3, 5.7, 9.9},
        {8.4, 2.8, 1.0}
    };
    float b[3][3] = {
        {4.1, 12.0, 7.3},
        {6.2, 5.3, 6.6},
        {1.5, 2.0, 1.0}
    };
    float **resultadoTeste = MatMul(a, b);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%f ", resultadoTeste[i][j]);
        }
        printf("\n");
    }
    return 0;
}