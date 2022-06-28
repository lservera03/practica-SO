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
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "ServerInfo.h"
#include "File.h"


char *tramaStartConexion(char *name, char *codipostal);

char *tramaConnectionCreated(int id);

char *tramaConnectionFailed();

char *tramaPhotoCorrect();

char *tramaPhotoNotCorrect();

char *tramaFinishConeixion(char *name, int id_user);

char *tramaSearch(char *name, int id_user, char *codipostal);

char *tramaSearchResponse(char *string);

char *tramaSearchPicture(char *nameFichero,  int size , char *MD5SUM);

char *tramaPhotoPicture(char *filename, int size, char *MD5SUM);

char *tramaPhotoRequest(char *id);

char *tramaPhotoNotFound();

char *sendDataPhoto(int photo_fd);

char *sendDataPhotoAtreides(int photo_fd);

char *sendResponse(int value);

char *MD5Generate(char *pathFoto);

int GetSizeFile (char *pathFoto);

char *GEtBinari( int photo_fd);

char *createOrigin(char string[]);


