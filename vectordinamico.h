#ifndef VECTORDINAMICO_H
#define VECTORDINAMICO_H

#include <stdbool.h>
#include <stddef.h>

#define KEY_LEN 20
#define MAX_VALUES 15 // == sizeof DINERO * 2

#define DINERO unsigned long
#define DINERO_F "%lu"

struct NODE
{
    char header[KEY_LEN];
    DINERO keys[MAX_VALUES];
    DINERO values[MAX_VALUES];
    size_t size;
};
typedef struct NODE TELEMENTO; /*Tipo de datos del vector*/

typedef struct
{
    TELEMENTO* datos;
    size_t tam;
} STVECTOR;
typedef STVECTOR* vectorD;


/*Funciones exportadas*/
/*Función CreaVector: asigna memoria y devuelve la asignación al vector*/
void CreaVector(vectorD* v1, int tam1);
/*Función AsignaVector: Llena una posición del vector con un valor*/
void AsignaVector(vectorD* v1, int posicion, TELEMENTO valor);
TELEMENTO Componentei(vectorD v1, int posicion);
void LiberaVector(vectorD* v1);
int tamano(vectorD v1);
bool EsNulo(vectorD v1);
void Aumentar(vectorD* v1, int n);


#endif /*VECTORDINAMICO_H*/
