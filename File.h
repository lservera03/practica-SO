#ifndef PRACTICA_FILE_H
#define PRACTICA_FILE_H

#include "ServerInfo.h"
#include <sys/fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <printf.h>


#define FREMEN_CONFIG 1
#define ATREIDES_CONFIG 2

int readConfigFile(char file[50], ServerInfo* serverInfo, int config);




#endif //PRACTICA_FILE_H
