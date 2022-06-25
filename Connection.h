#ifndef PRACTICA_CONNECTION_H
#define PRACTICA_CONNECTION_H


typedef struct {
    int user_id;
    int file_descriptor;
    pthread_t thread;
} Connection;


#endif //PRACTICA_CONNECTION_H
