//
// Created by xavie on 21/06/2022.
//

#ifndef PRACTICASO_PLOTCREATION_H
#define PRACTICASO_PLOTCREATION_H

#endif //PRACTICASO_PLOTCREATION_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "ServerInfo.h"
#include "File.h"

typedef struct {
    char origin[15];
    char type;
    char data[240];
} Frame;


char *tramaStartConexion(char *name , char *codipostal);
char *createOrigin(char string[]);
void readFrame(const char *frame);

