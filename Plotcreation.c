#include "Plotcreation.h"
#include <stdio.h>
#include "unistd.h"

#define printF(x) write(1, x, strlen(x))


char *createOrigin(char string[]) {
    char *trama = malloc(256 * sizeof(*trama));
    strcpy(trama, string);

    for (int i = strlen(string); i < 15; i++) {
        trama[i] = '\0';
    }

    return trama;
}


char *completeDataTrama(char *trama, char *dades) {

    for (unsigned int i = 0; i < strlen(dades); i++) {
        trama[i + 16] = dades[i];
    }

    for (int i = 16 + strlen(dades); i < 256; i++) {
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

    trama = completeDataTrama(trama, dades);


    return trama;
}

char *tramaConnectionCreated(int id) {
    char *trama;
    char aux[20];


    //Know the length of the integer to create string with that size
    int id_length = sprintf(aux, "%d", id);

    char *dades = (char *) malloc(id_length);

    trama = createOrigin("ATREIDES");
    trama[15] = 'O';

    sprintf(dades, "%d", id);

    trama = completeDataTrama(trama, dades);


    return trama;
}


char *tramaFinishConeixion(char *name, char *id) {
    char *trama;
    char *dades = (char *) malloc(sizeof(name) + sizeof(id));

    trama = createOrigin("FREMEN");
    trama[15] = 'Q';

    sprintf(dades, "%s*%s", name, id);

    trama = completeDataTrama(trama, dades);

    return trama;
}


char *tramaSearch(char *name, char *id, char *codipostal) {
    char *trama;
    char *dades = (char *) malloc(sizeof(name) + sizeof(id) + sizeof(codipostal));

    trama = createOrigin("FREMEN");
    trama[15] = 'S';

    sprintf(dades, "%s*%s*%s", name, id, codipostal);

    trama = completeDataTrama(trama, dades);

    return trama;
}


char *tramaSearchPicture(char *nameFichero, char *size, char *MD5SUM) {
    char *trama;
    char *dades = (char *) malloc(sizeof(nameFichero) + sizeof(size) + sizeof(MD5SUM));

    if (strlen(nameFichero) > 30) return "1";
    if (strlen(MD5SUM) > 32) return "2";

    trama = createOrigin("FREMEN");
    trama[15] = 'F';

    sprintf(dades, "%s*%s*%s", nameFichero, size, MD5SUM);

    trama = completeDataTrama(trama, dades);

    return trama;
}

char *tramaPhotoPeticion(char *id) {
    char *trama;
    char *dades = (char *) malloc(sizeof(id));

    trama = createOrigin("FREMEN");
    trama[15] = 'P';

    sprintf(dades, "%s", id);

    trama = completeDataTrama(trama, dades);

    return trama;
}

char *sendDataPhoto(char *dadesBinarias) {
    char *trama;
    char *dades = (char *) malloc(sizeof(dadesBinarias));

    trama = createOrigin("FREMEN");
    trama[15] = 'D';

    //TODO no es pot concatenar una string amb si mateixa (arreglar)
    //sprintf(dades, "%s", dades);

    trama = completeDataTrama(trama, dades);

    return trama;

}

char *MD5Generate(char *pathFoto) {
    int fill;

    fill = fork();
    if (fill > 0) {
       // wait(NULL);
    } else {

      /*  char *args[3];

        args[0] = "md5sum";
        args[1] = pathFoto;
        args[2] = NULL;

        execvp(args[0], args);
*/

        execl("/bin/md5sum" ,"md5sum", pathFoto ,NULL);
    }
    return NULL;
}


