#include "Plotcreation.h"
#include <stdio.h>
#include "unistd.h"

#define printF(x) write(1, x, strlen(x))



char *createOrigin(char string[]) {
    char *trama = (char * )malloc(sizeof(char) * 256);

    sprintf(trama, "%s", string);

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

char *tramaStartConexion(char *nom, char *codipostal){
    char *trama;
    char *dades = (char *) malloc(sizeof(char) * (strlen(nom) + strlen(codipostal) + 2));

    trama = createOrigin("FREMEN");
    trama[15] = 'C';

    sprintf(dades, "%s*%s", nom, codipostal);

    trama = completeDataTrama(trama, dades);

    free(dades);
    return trama;
}



char *tramaPhotoCorrect() {
    char *trama;
    char *dades = (char *) malloc(sizeof(char) + strlen("IMAGE OK"));

    trama = createOrigin("FREMEN");
    trama[15] = 'I';

	strcpy(dades, "IMAGE OK");
	
    trama = completeDataTrama(trama, dades);

    free(dades);
    return trama;
}



char *tramaPhotoNotCorrect() {
    char *trama;
    char *dades = (char *) malloc(sizeof(char) + strlen("IMAGE KO"));

    trama = createOrigin("FREMEN");
    trama[15] = 'R';

	strcpy(dades, "IMAGE KO");
	
    trama = completeDataTrama(trama, dades);

    free(dades);
    return trama;
}



char *tramaConnectionCreated(int id) {
    char *trama;
    char aux[20];


    //Know the length of the integer to create string with that size
    int id_length = sprintf(aux, "%d", id);

    char *dades = (char *) malloc(sizeof(char) + (id_length + 1));

    trama = createOrigin("ATREIDES");
    trama[15] = 'O';

    sprintf(dades, "%d", id);

    trama = completeDataTrama(trama, dades);

    free(dades);
    return trama;
}



char *tramaPhotoNotFound() {
    char *trama;
	char *dades = (char *) malloc(sizeof(char) * strlen("FILE NOT FOUND"));

    trama = createOrigin("ATREIDES");
    trama[15] = 'F';

    sprintf(dades, "%s", "FILE NOT FOUND");

    trama = completeDataTrama(trama, dades);
    free(dades);
    return trama;
}

char *tramaFinishConeixion(char *name, int id_user) {
    char *trama;
    char aux[50];

    //Get id length for malloc
    int id_length = sprintf(aux, "%d", id_user);


    char *dades = (char *) malloc(sizeof(char) * (strlen(name) + id_length + 1));

    trama = createOrigin("FREMEN");
    trama[15] = 'Q';

    sprintf(dades, "%s*%d", name, id_user);

    trama = completeDataTrama(trama, dades);
    free(dades);
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
    free(dades);
    return trama;
}


char *tramaSearchResponse(char *string) {
    char *trama;

    char *dades = (char *) malloc(sizeof(char) * strlen(string) + 1);

    trama = createOrigin("ATREIDES");
    trama[15] = 'L';

    sprintf(dades, "%s", string);

    trama = completeDataTrama(trama, dades);
    free(dades);
	return trama;
}


char *tramaSearchPicture(char *nameFichero, int size, char *MD5SUM) {
    char *trama;

    char *dades = (char *) malloc(sizeof(char) * (strlen(nameFichero) +1 + sizeof(size) + strlen(MD5SUM) + 2));

    if (strlen(nameFichero) > 30) return "1";

    trama = createOrigin("FREMEN");
    trama[15] = 'F';

    sprintf(dades, "%s*%d*%s", nameFichero, size, MD5SUM);

    trama = completeDataTrama(trama, dades);

    free(dades);
    return trama;
}




char *tramaPhotoPicture(char *filename, int size, char *MD5SUM) {
    char *trama;
    char *dades = (char *) malloc(sizeof(char) * (strlen(filename) + sizeof(size) + strlen(MD5SUM) + 3));

    if (strlen(filename) > 30) return "1";

    trama = createOrigin("ATREIDES");
    trama[15] = 'F';

    sprintf(dades, "%s*%d*%s", filename, size, MD5SUM);

    trama = completeDataTrama(trama, dades);

    free(dades);
    return trama;

}



char *tramaPhotoRequest(char *id) {
    char *trama;
    char *dades = (char *) malloc(sizeof(id));

    trama = createOrigin("FREMEN");
    trama[15] = 'P';

    sprintf(dades, "%s", id);

    trama = completeDataTrama(trama, dades);
    free(dades);
    return trama;
}



char *completeDataPhoto(char *trama, char buffer[240]){

	for(int i = 0; i < 240; i++){
		trama[i + 16] = buffer[i];
	}

	return trama;
}



char *sendDataPhotoAtreides(int photo_fd){
	char *trama;

	trama = createOrigin("ATREIDES");
	trama[15] = 'D';

	char buffer[240];

	read(photo_fd, buffer, 240);

	trama = completeDataPhoto(trama, buffer);

	return trama;
}


char *sendDataPhoto(int photo_fd) {
    char *trama;

    trama = createOrigin("FREMEN");
    trama[15] = 'D';

	char buffer[240];

	read(photo_fd, buffer, 240);

    trama = completeDataPhoto(trama, buffer);

    return trama;
}

char *sendResponse(int value){
    char *trama;
    char *buffer;

    trama = createOrigin("ATREIDES");
    if (value == 1){
        trama[15] = 'I';
		buffer = (char *) malloc(sizeof(char) * (strlen("IMAGE OK") + 1));
       	strcpy(buffer, "IMAGE OK");
    } else {
        trama[15] = 'R';
		buffer = (char *) malloc(sizeof(char) * (strlen("IMAGE KO") + 1));
        strcpy(buffer, "IMAGE KO");
    }

    trama = completeDataTrama(trama, buffer);



	free(buffer);
    return trama;
}


char *MD5Generate(char *pathFoto) {
    pid_t pid = 0;
    int pipefd[2];

    pipe(pipefd); //create a pipe
    pid = fork(); //spawn a child process

    if (pid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execl("/bin/md5sum", "md5sum", pathFoto, (char *) 0);
    } else { 
        close(pipefd[1]);
        char *md5Hash = malloc(256 * sizeof(char));

        read(pipefd[0], md5Hash, 256);
        close(pipefd[0]);
		
		strtok(md5Hash, " ");

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

char *GEtBinari( int photo_fd ) {

    char *dadesBinarias = malloc(240 * sizeof(*dadesBinarias));
    memset(dadesBinarias, 0, 240);

    read(photo_fd,dadesBinarias,240);

    return  dadesBinarias;
}
