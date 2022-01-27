#ifndef PRACTICA_COMMAND_H
#define PRACTICA_COMMAND_H

#include <printf.h>
#include "string.h"
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef struct{
    char **arguments;
    int num_arguments;
} Command;


int executeCommand(char string[]);


void createCommand(char string[]);

void freeMemoryCommand();



#endif //PRACTICA_COMMAND_H
