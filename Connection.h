#ifndef PRACTICA_CONNECTION_H
#define PRACTICA_CONNECTION_H


typedef struct {
    User user;
    int file_descriptor;
    pthread_t thread;
} Connection;


#endif //PRACTICA_CONNECTION_H
