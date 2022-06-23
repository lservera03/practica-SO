
#include "Plotcreation.h"

#define printF(x) write(1, x, strlen(x))

#include <stdio.h>

/*
void readFrame(const char *trama) {
    Frame *frame;


    for (int i = 0; i < 15; i++) {
        frame->origin[i] = trama[i];
    }

    frame->type = trama[15];

    for (int i = 16; i < 256; i++) {
        frame->data[i] = trama[i];
    }
}
*/

char *createOrigin(char string[]) {
    char *trama = malloc(256 * sizeof(*trama));
    strcpy(trama, string);


    for (int i = strlen(string); i < 15; i++) {
        trama[i] = '\0';
    }

    return trama;
}

char *tramaStartConexion(char *nom, char *codipostal) {
    char *trama;
    char *dades = (char*) malloc(sizeof(nom)+ sizeof(codipostal));

    printf("DATA : %s + %s\n", nom , codipostal);

    trama = createOrigin("FREMEN");
    trama[15] = 'C';

    sprintf(dades, "%s*%s", nom, codipostal);

    printf("dades : %s + %lu\n", dades, strlen(dades));

    for (int i = 0; i < strlen(dades); i++) {
        trama[i+16] = dades[i];
    }

    for (int i = 16 + strlen(dades); i < 256; i++) {
        trama[i] = '\0';
    }

    for (int i = 0; i < 256; i++) {
        printf("%c + %d \n", trama[i], i);
    }

    return trama;
}







