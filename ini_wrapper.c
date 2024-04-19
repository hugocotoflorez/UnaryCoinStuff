#include "ini_wrapper.h"
#include "vectordinamico.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * tiny ".ini" wrapper.
 * Author: Hugo Coto Florez
 */

enum TOKENS
{
    HEADER_START = 1,
    HEADER_END,
    WHITESPACE,
    NULLCHAR,
    EQUAL,
    EOL,
};

enum STATE
{
    NONE,
    GET_HEADER,
    GET_KEY,
    GET_VALUE,
};

static int lookup[] = {
    ['[']  = HEADER_START,
    [']']  = HEADER_END,
    [' ']  = WHITESPACE,
    ['\0'] = NULLCHAR,
    ['=']  = EQUAL,
    ['\n'] = EOL,
    [127]  = 0,
};


void check_error()
{
    if(errno)
    {
        perror("Error");
        exit(errno);
    }
}


void die(const char* e)
{
    puts(e);
    exit(-1);
}


void get_data(FILE* file, vectorD* data)
{
    char c;                   // file current char
    struct NODE current;      // current node
    char buff[BUFF_SIZE + 1]; // buffer (the +1 is to save a \0)
    int i            = 0;     // buffer index
    int j            = 0;     // key index
    enum STATE state = NONE;  // current state
    int vec_index    = -1;     // Index to save into vector

    while((c = getc(file)) != EOF)
    {
        switch(lookup[c])
        {
            case HEADER_START:
                state = GET_HEADER;
                i     = 0;
                j     = 0;
                ++vec_index;

                break;
            case HEADER_END:
                state   = NONE;
                buff[i] = '\0';
                if(IW_VERBOSE)printf("HEADER: %s\n", buff);
                strcpy(current.key, buff);

                break;
            case WHITESPACE:
                // on whitespace: dont do anything
                break;
            case NULLCHAR:
                //puts("NULLCHAR");
                // TODO WTF IF THIS
                // eof? of error idk
                break;
            case EQUAL:
                state   = GET_VALUE;
                buff[i] = '\0';
                i       = 0;
                if(IW_VERBOSE)printf("|  KEY[%d]: %10s", j,buff);
                current.values[j] = strtol(buff,NULL,10);
                check_error();
                current.values_n = j+1;
                break;
            case EOL:
                if(state == GET_VALUE)
                {
                    buff[i] = '\0';
                    if(IW_VERBOSE)printf("|  VALUE: %s\n", buff);
                    current.amount[j++] = strtol(buff, NULL, 10);
                    check_error();
                    i                   = 0;
                    state               = NONE;
                    // se incrementa al abrir un header
                    if(tamano(*data) == vec_index )
                        Aumentar(data,1);
                    AsignaVector(data, vec_index, current);
                                    }
                else if(state != NONE)
                    die("Unexpected end of line");

                break;
            default:
                if(state == GET_HEADER || state == GET_KEY || state == GET_VALUE)
                {
                    if(i == BUFF_SIZE)
                        die("Header too large");
                    buff[i++] = c;
                }
                else if(state == NONE)
                {
                    state = GET_KEY;
                    i     = 0;
                    buff[i++] = c;
                }
                break;
        }
    }
}





void update_data(FILE* f, vectorD data)
{
    for (int i = 0; i < tamano(data); ++i)
    {
        fprintf(f, "[%s]\n", Componentei(data, i).key);
        printf( "[%s]\n", Componentei(data, i).key);
        for(int j = 0; j < Componentei(data, i).values_n; ++j)
        {
            fprintf(f, "%u = %u\n", Componentei(data, i).values[j], Componentei(data, i).amount[j]);
            printf( "%u = %u\n", Componentei(data, i).values[j], Componentei(data, i).amount[j]);
        }
    }
}
