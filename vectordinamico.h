#ifndef VECTORDINAMICO_H
#define VECTORDINAMICO_H

#define KEY_LEN 20
#define MAX_VALUES 8

struct NODE
{
    char key[KEY_LEN];
    int values[MAX_VALUES];
    int amount[MAX_VALUES];
    int values_n;
};

typedef struct NODE TELEMENTO; /*Tipo de datos del vector*/
typedef void* vectorD;         /*tipo opaco*/

void inicializaVectorRand(vectorD* v1);
void swap(vectorD* pvector, int m, int n);
void print_vector(vectorD vector);
/*Funciones exportadas*/
/*Función CreaVector: asigna memoria y devuelve la asignación al vector*/
void CreaVector(vectorD* v1, int tam1);
/*Función AsignaVector: Llena una posición del vector con un valor*/
void AsignaVector(vectorD* v1, int posicion, TELEMENTO valor);
TELEMENTO Componentei(vectorD v1, int posicion);
void LiberaVector(vectorD* v1);
int tamano(vectorD v1);
int EsNulo(vectorD v1);
void Aumentar(vectorD* v1, int n);


#endif /*VECTORDINAMICO_H*/
