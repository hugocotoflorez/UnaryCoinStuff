#ifndef _INI_WRAPPER
#define _INI_WRAPPER

#include "vectordinamico.h"
#include <stdio.h>
#define BUFF_SIZE 20
#define IW_VERBOSE 0


// functions
void get_data(FILE* file, vectorD* data);
void die(const char* e);
void check_error();
void update_data(FILE* f, vectorD data);


#endif
