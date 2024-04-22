#include "ini_wrapper.h"
#include "vectordinamico.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

// Defined in vectordinamico.h
// #define DINERO
// #define DINERO_F

void check_orientation(DINERO* c, DINERO* s, size_t n)
{
    /*
     * ! La lista de monedas (c) y stock (s) deben estar ordenadas
     * y en el mismo sentido
     *
     * Esta funcion le da la vuelta a la las listas si estan ordenadas
     * de mayor a menor, ya que el algoritmo de cambio requiere los valores
     * mas grandes en las posiciones mas grandes.
     */
    DINERO aux;
    if(c[0] > c[n - 1])
        for(int i = 0; i < n / 2; ++i)
        {
            aux          = c[i];
            c[i]         = c[n - i - 1];
            c[n - i - 1] = aux;
            aux          = s[i];
            s[i]         = s[n - i - 1];
            s[n - i - 1] = aux;
        }
}

DINERO calcular_cambio(DINERO* m, size_t n, DINERO c[n], DINERO s[n])
{
    /**
     * Calculador de cambio
     * (c) Hugo Coto Florez
     *
     * - *m: Total de dinero, si m es 0 se consiguio una solucion,
     *     sino el resto queda en m.
     * - n: Tamano del array con los valores
     * - c[n]: Array con los valores de las monedas, de menor a mayor
     *
     * -> v: solucion voraz (no se asegura que se encuentre)
     *
     * Complejidad espacial: O(1)
     * Complejidad temporal: O(n)
     *
     * Limitaciones:
     *
     *  - Maxima cantidad monedas
     *  int:
     *  sizeof int = 4 bytes = 32bits
     *  32/4 = 8 digitos hexadecimales = 8 posibles valores
     *
     * - Maximo cambio
     * Fxxxxxxx
     * 15 de c[n-1] + n de cada tipo mientras no llegue al valor de la siguiente
     * (Limitado a 15 de c[n-1] por comodidad)
     */
    DINERO v = 0;
    DINERO k, l;

    if(c[0] > c[n - 1])
        die("Unsorted coin keys");
    if(sizeof v * 2 < n)
        die("Too much keys");
    // Cantidad a cambiar demasiado grande para el sistema monetario elegido
    if(*m > c[n - 1] * 15)
        return -1;

    for(int i = n - 1; i >= 0 && *m != 0; i--)
    {
        /*
         * Numero de monedas del tipo c[i] necesitadas: *m/c[i]
         * Maximas monedas que puede dar: min(s[i], 15):
         *      a) s[i] > 15 :  15
         *      b) s[i] < 15 : s[i]
         * Monedas que va a dar:  min(s[i], 15, *m/c[i])
         * En el digito i de v se guarda la cantidad de monedas del tipo c[i]
         * El dinero a cambiar se reduce la cantidad por el valor
         * El stock se reduce el valor
         */
        k = *m/c[i];
        l = (s[i] >> 4)? 0xF : s[i];
        k = (k < l)? k : l;
        v |= k << 4 * i;
        *m -= k * c[i];
        s[i] -= k;
    }
    return v;
}

void interpr(DINERO v, size_t n, DINERO c[n], DINERO* k)
{
    int m;
    if(c[0] > c[n - 1])
        die("Unsorted coin keys");

    switch(v)
    {
        /*
        case 0:
            if(*k)
                printf("Dinero no devuelto: " DINERO_F "\n", *k);
            break;
            */
        case -1:
            puts("Too big ammount");
            break;
        default:
            for(int i = 0; i < n; i++){
                m = v & 0xF;
                v >>= 4;
                if (m)
                    printf("%d de " DINERO_F "\n", m, c[i]);
                }
            if(*k)
                printf("Dinero no devuelto: " DINERO_F "\n", *k);
            break;
    }
}


int main(int argc, char** argv)
{
    DINERO d;            // dinero a cambiar
    vectorD data = NULL; // valores de monedas y stock
    TELEMENTO k;         // nodo para actualizar el stock
    int i;               // indicador de moneda a usar
    size_t n;            // cantidad de valores de moneda distintos
    size_t t;            // cantidad de monedas distinto
    DINERO* c;           // valores de monedas
    DINERO* s;           // stock de monedas
    FILE* f;             // Archivo con los datos de las monedas
    errno = 0;           // errno
    if(argc != 2)
        die("Data file not loaded");
    f = fopen(argv[1], "r");
    check_error();
    get_data(f, &data);
    fclose(f);
    t = tamano(data);
    if(t == 0)
        die("No coin data in data file");
    for(i = 0; i < t; ++i)
        printf("[%d] %s\n", i, Componentei(data, i).header);
    puts("Select coin:");
    do
    {
        scanf(" %d", &i);
    } while(i < 0 || i >= t);
    n = Componentei(data, i).size;
    c = Componentei(data, i).keys;
    s = Componentei(data, i).values;
    // Se asume que los valores de las monedas estan ordenados
    check_orientation(c, s, n);
    while(1)
    {
        puts("Centimos a cambiar: ");
        scanf(" " DINERO_F, &d);
        if(!d)
            break;
        interpr(calcular_cambio(&d, n, c, s), n, c, &d);
    }
    // ----------------------------------------------- //
    // Actializamos el vector con el stock actual
    strncpy(k.header, Componentei(data, i).header, KEY_LEN);
    k.size = n;
    for(int l = 0; l < n; l++)
    {
        k.keys[l]   = c[l];
        k.values[l] = s[l];
    }
    AsignaVector(&data, i, k);
    // ----------------------------------------------- //
    f = fopen(argv[1], "w");
    check_error();
    update_data(f, data);
    LiberaVector(&data);
    fclose(f);
}
