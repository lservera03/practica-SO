#ifndef PRACTICA_FRAME_H
#define PRACTICA_FRAME_H

#include <string.h>

#define ORIGIN_LENGTH 15
#define DATA_LENGTH 240


typedef struct {
    char origin[ORIGIN_LENGTH];
    char type;
    char data[DATA_LENGTH];
} Frame;


Frame createFrameFromString(char string[256]);


#endif //PRACTICA_FRAME_H
