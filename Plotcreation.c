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



char *tramaFinishConeixion(char *name, int id_user) {
    char *trama;
    char aux[50];

    //Get id length for malloc
    int id_length = sprintf(aux, "%d", id_user);


    char *dades = (char *) malloc(sizeof(name) + id_length);

    trama = createOrigin("FREMEN");
    trama[15] = 'Q';

    sprintf(dades, "%s*%d", name, id_user);

    trama = completeDataTrama(trama, dades);

    return trama;
}


char *tramaSearch(char *name, int id_user, char *codipostal) {
    char *trama;
    char aux[50];

    //Get id length for the malloc
    int id_length = sprintf(aux, "%d", id_user);

    char *dades = (char *) malloc(sizeof(name) + id_length + sizeof(codipostal));

    trama = createOrigin("FREMEN");
    trama[15] = 'S';

    sprintf(dades, "%s*%d*%s", name, id_user, codipostal);

    trama = completeDataTrama(trama, dades);

    return trama;
}


char *tramaSearchResponse(char *string) {
    char *trama;

    char *dades = (char *) malloc(sizeof(char) * strlen(string));

    trama = createOrigin("ATREIDES");
    trama[15] = 'L';

    sprintf(dades, "%s", string);

    trama = completeDataTrama(trama, dades);

	return trama;
}


char *tramaSearchPicture(char *nameFichero, int size, char *MD5SUM) {
    char *trama;
    char *dades = (char *) malloc(sizeof(nameFichero) + sizeof(size) + sizeof(MD5SUM));

    if (strlen(nameFichero) > 30) return "1";
    if (strlen(MD5SUM) > 32) return "2";

    trama = createOrigin("FREMEN");
    trama[15] = 'F';

    sprintf(dades, "%s*%d*%s", nameFichero, size, MD5SUM);

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

    sprintf(dades, "%s", dadesBinarias);

    trama = completeDataTrama(trama, dades);

    return trama;

}

char *MD5Generate(char *pathFoto) {
    pid_t pid = 0;
    int pipefd[2];
    char *args[] = {"md5sum", pathFoto, NULL};

    pipe(pipefd); //create a pipe
    pid = fork(); //spawn a child process

    if (pid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execvp(args[0], args);
    } else {
        close(pipefd[1]);
        char *md5Hash = malloc(32 * sizeof(*md5Hash));

        read(pipefd[0], md5Hash, 32);
        strtok(md5Hash, " ");

        close(pipefd[0]);
        return md5Hash;
    }

    return NULL;
}

int GetSizeFile (char *pathFoto) {
    struct stat st;

    stat(pathFoto, &st);
    int size = st.st_size;

    return size;
}

char *GEtBinari( int photo_fd) {

    char *dadesBinarias = malloc(240 * sizeof(*dadesBinarias));

    read(photo_fd,dadesBinarias,240);

    //printF(dadesBinarias);
//    for (int i = 0; i < 240; i++) {
//        printf("%c",dadesBinarias[i]);
//    }
    return  dadesBinarias;
}