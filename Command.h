#ifndef PRACTICA_COMMAND_H
#define PRACTICA_COMMAND_H

#include <printf.h>
#include "string.h"
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include "ServerInfo.h"
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Plotcreation.h"

typedef struct {
    char **arguments;
    int num_arguments;
} Command;


void create_connection_atreides();

int executeCommand(char string[], ServerInfo *serverInfo);


void createCommand(char string[]);

void freeMemoryCommand();

int checkIfIsNumber(char *string[]);


#endif //PRACTICA_COMMAND_H
