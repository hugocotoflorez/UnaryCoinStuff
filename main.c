#include "ini_wrapper.h"
#include "vectordinamico.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define DINERO unsigned int
#define DINERO_F "%u"

void check_orientation(DINERO* c, DINERO* s, int n)
{
    unsigned int aux;
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
     * Maxima cantidad monedas
     * int:
     *  sizeof int = 4 bytes = 32bits
     *  32/4 = 8 digitos hexadecimales = 8 posibles valores
     *
     * Maximo cambio
     * Fxxxxxxx
     * 15 de c[n-1] + n de cada tipo mientras no llegue al valor de la siguiente
     * (Limitado a 15 de c[n-1])
     */
    DINERO v = 0;
    DINERO k;

    if(c[0] > c[n - 1])
        die("Unsorted coin values");
    if(sizeof v * 2 < n)
        die("Too much values");
    if(*m > c[n - 1] * 15)
        return -1;

    for(int i = n - 1; i >= 0 && *m != 0; i--)
    {
        /*
         * Monedas del tipo c[i]:
         *      min( *m/c[i] , s[i], 15 )
         *      15 = 0xF -> min(15, s[i]) = s[i] & 15
         *      min( *m/c[i] , s[i], 15 ) -> min (*m/c[i], s[i] & 0x1111)
         */
        k = (*m / c[i] < (s[i] & 0xF)) ? *m / c[i] : s[i] & 0xF;
        printf("K: %d (%x)\n", k, k);
        v |= k << 4 * i;
        *m -= k * c[i];
        s[i] -= k;
        printf("Stock de %u: %d -> %d\n", c[i], s[i] + k, s[i]);
    }
    printf("M: %u\n", *m);
    return v;
}

void interpr(DINERO v, size_t n, DINERO c[n], DINERO* k)
{
    DINERO m;
    if(c[0] > c[n - 1])
        die("Unsorted coin values");

    switch(v)
    {
        case 0:
            break;
        case -1:
            puts("Da billete");
            break;
        default:
            for(int i = n - 1; i >= 0; i--)
                if((m = (v & 0xF << 4 * i)))
                {
                    m >>= 4 * i;
                    printf(DINERO_F " de %d\n", m, c[i]);
                }
            if(*k)
                printf("Dinero no devuelto: %u\n", *k);
    }
}


int main(int argc, char** argv)
{
    DINERO d;
    vectorD data = NULL;
    TELEMENTO k;
    int aux, i;
    size_t n;
    DINERO* c;
    DINERO* s;
    errno = 0;
    if(argc != 2)
        die("Data file not loaded");
    FILE* f = fopen(argv[1], "r");
    check_error();
    get_data(f, &data);
    fclose(f);
    f = fopen(argv[1], "w");
    check_error();

    int t = tamano(data);
    for(i = 0; i < t; ++i)
        printf("[%d] %s\n", i, Componentei(data, i).key);
    puts("Select coin:");
    do
    {
        scanf(" %d", &i);
    } while(i < 0 || i >= t);

    n = Componentei(data, i).values_n;
    c = Componentei(data, i).values;
    s = Componentei(data, i).amount;

    // its assumed that coin values are sorted
    check_orientation(c, s, n);

    while(1)
    {
        puts("Centimos a cambiar: ");
        scanf(" " DINERO_F, &d);
        if(!d)
            break;
        interpr(calcular_cambio(&d, n, c, s), n, c, &d);
    }
    strncpy(k.key, Componentei(data, i).key, KEY_LEN);
    k.values_n = n;
    for(int l = 0; l < n; l++)
    {
        k.values[l] = c[l];
        k.amount[l] = s[l];
    }
    AsignaVector(&data, i, k);
    update_data(f, data);
    LiberaVector(&data);
    fclose(f);
}
