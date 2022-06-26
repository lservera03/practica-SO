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


char *tramaStartConexion(char *name, char *codipostal);

char *tramaConnectionCreated(int id);

char *tramaFinishConeixion(char *name, int id_user);

char *tramaSearch(char *name, int id_user, char *codipostal);

char *tramaSearchPicture(char *nameFichero, char *size , char *MD5SUM);

char *tramaPhotoPeticion(char *id);

char *sendDataPhoto(char *dadesBinarias);

char *createOrigin(char string[]);


