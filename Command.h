#ifndef PRACTICA_COMMAND_H
#define PRACTICA_COMMAND_H

#include <printf.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "ServerInfo.h"
#include "Frame.h"
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Plotcreation.h"
#include <signal.h>


typedef struct {
    char **arguments;
    int num_arguments;
} Command;


void create_connection_atreides();

int executeCommand(char string[], ServerInfo *serverInfo);

void RSI_SIGINT();

void createCommand(char string[]);

void freeMemoryCommand();


#endif //PRACTICA_COMMAND_H
