#include "Plotcreation.h"

#define printF(x) write(1, x, strlen(x))



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
    char *dades = (char *) malloc(sizeof(nom) + sizeof(codipostal));

    trama = createOrigin("FREMEN");
    trama[15] = 'C';

    sprintf(dades, "%s*%s", nom, codipostal);

    for (int i = 0; i < strlen(dades); i++) {
        trama[i + 16] = dades[i];
    }

    for (int i = 16 + strlen(dades); i < 256; i++) {
        trama[i] = '\0';
    }


    return trama;
}







