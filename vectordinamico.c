#include "vectordinamico.h"
#include <stdbool.h>
#include <stdlib.h>

#define KEY_LEN 20


bool EsNulo(vectorD v1)
{
    if(v1 == 0)
        return true;
    return false;
}

/*Función LiberaVector: libera la memoria del vector si no es nulo*/
void LiberaVector(vectorD* v1)
{
    if(!EsNulo(*v1))
    {
        /*Primero liberamos el bloque al que apunta datos*/
        free((*v1)->datos);
        /*Y después liberamos la estructura*/
        free(*v1);
        /*Volvemos a inicializar el puntero a 0*/
        *v1 = 0;
    }
}

/*Función CreaVector: asigna memoria y devuelve la asignación al vector*/
void CreaVector(vectorD* v1, int tam1)
{
    LiberaVector(v1);
    *v1          = (vectorD)malloc(sizeof(STVECTOR));
    (*v1)->datos = (TELEMENTO*)malloc(tam1 * sizeof(TELEMENTO));
    (**v1).tam   = tam1;
}

void Aumentar(vectorD* v1, int n)
{
    if(!EsNulo(*v1))
    {
        (**v1).tam = (**v1).tam += n;
        (*v1)->datos = (TELEMENTO*)realloc((*v1)->datos, (**v1).tam * sizeof(TELEMENTO));
    }
    else
        CreaVector(v1, n);
}


/*Función AsignaVector: Llena una posición del vector con un valor*/
void AsignaVector(vectorD* v1, int posicion, TELEMENTO valor)
{
    *((*v1)->datos + posicion) = valor;
}


TELEMENTO Componentei(vectorD v1, int posicion)
{
    if(!EsNulo(v1))
        if(posicion < v1->tam)
            return *(v1->datos + posicion);
}


int tamano(vectorD v1)
{
    if(EsNulo(v1))
        return 0;
    return v1->tam;
}
