#ifndef PRACTICA_SERVERINFO_H
#define PRACTICA_SERVERINFO_H


#define MAX_IP_STRING 20
#define MAX_DIRECTORY_STRING 50


typedef struct {
    int seconds;
    char ip[MAX_IP_STRING];
    int port;
    char directory[MAX_DIRECTORY_STRING];
} ServerInfo;




#endif //PRACTICA_SERVERINFO_H
