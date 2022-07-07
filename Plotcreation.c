#include "Plotcreation.h"

#define printF(x) write(1, x, strlen(x))


/**
 * Function that allows to form the origin section of the Frame.
 * @param string String containing the origin of the Frame.
 * @return Pointer to the frame string formed.
 */
char *createOrigin(char string[]) {
    char *trama = (char *) malloc(sizeof(char) * 256);

    sprintf(trama, "%s", string);

    for (int i = strlen(string); i < 15; i++) {
        trama[i] = '\0';
    }

    return trama;
}


/**
 * Function that allows to form the data section of the frame.
 * @param trama Pointer to a string frame already formed.
 * @param dades Pointer to a string containing the info to place into the frame.
 * @return Pointer to the frame string.
 */
char *completeDataTrama(char *trama, char *dades) {

    for (unsigned int i = 0; i < strlen(dades); i++) {
        trama[i + 16] = dades[i];
    }

    for (int i = 16 + strlen(dades); i < 256; i++) {
        trama[i] = '\0';
    }

    return trama;
}


/**
 * Function that allows to create the start connection frame.
 * @param nom Receives the username to place into the frame.
 * @param codipostal Receives the postal code to place into the frame.
 * @return Pointer to the frame string.
 */
char *tramaStartConexion(char *nom, char *codipostal) {
    char *trama;
    char *dades = (char *) malloc(sizeof(char) * (strlen(nom) + strlen(codipostal) + 2));

    trama = createOrigin("FREMEN");
    trama[15] = 'C';

    sprintf(dades, "%s*%s", nom, codipostal);

    trama = completeDataTrama(trama, dades);

    free(dades);

    return trama;
}


/**
 * Function that allows to create the correct picture received by Fremen frame.
 * @return Pointer to the frame string.
 */
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


/**
 * Function that allows to create the incorrect picture received by Fremen frame.
 * @return Pointer to the frame string.
 */
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


/**
 * Function that allows to create the connection created correctly by Atreides frame.
 * @param id Integer containing the ID assigned to the user that requested connection.
 * @return Pointer to the frame string.
 */
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

/**
 * Function that allows to create the picture not found by Atreides frame.
 * @return Pointer to the frame string.
 */
char *tramaPhotoNotFound() {
    char *trama;
    char *dades = (char *) malloc(sizeof(char) * strlen("FILE NOT FOUND") + 1);

    trama = createOrigin("ATREIDES");
    trama[15] = 'F';

    sprintf(dades, "%s", "FILE NOT FOUND");

    trama = completeDataTrama(trama, dades);

    free(dades);

    return trama;
}


/**
 * Function that allows to create the finish connection request by Fremen frame.
 * @param name String containing the username.
 * @param id_user Integer containing the ID of the user.
 * @return Pointer to the frame string.
 */
char *tramaFinishConeixion(char *name, int id_user) {
    char *trama;
    char aux[50];

    //Get id length for malloc
    int id_length = sprintf(aux, "%d", id_user);

    char *dades = (char *) malloc(sizeof(char) * (strlen(name) + id_length + 2));

    trama = createOrigin("FREMEN");
    trama[15] = 'Q';

    sprintf(dades, "%s*%d", name, id_user);

    trama = completeDataTrama(trama, dades);

    free(dades);

    return trama;
}


/**
 * Function that allows to create the search by postal code frame.
 * @param name String containing the username that makes the request.
 * @param id_user Integer containing the ID of the user that makes the request.
 * @param codipostal String containing the postal code to search.
 * @return Pointer to the frame string.
 */
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


/**
 * Function that allows to create the search by postal code response by Atreides frame.
 * @param string String containing all the users' info found.
 * @return Pointer to the frame string.
 */
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


/**
 * Function that allows to create the send picture first frame containing the basic info of the picture.
 * @param nameFichero String containing the name of the picture to send.
 * @param size Integer containing the size of the picture to send.
 * @param MD5SUM String containing the MD5SUM of the picture to send.
 * @return Pointer to the frame string.
 */
char *tramaSendPicture(char *nameFichero, int size, char *MD5SUM) {
    char *trama;

    char *dades = (char *) malloc(sizeof(char) * (strlen(nameFichero) + sizeof(size) + strlen(MD5SUM) + 5));

    if (strlen(nameFichero) > 30) return "1";

    trama = createOrigin("FREMEN");
    trama[15] = 'F';

    sprintf(dades, "%s*%d*%s", nameFichero, size, MD5SUM);

    trama = completeDataTrama(trama, dades);

    free(dades);

    return trama;
}


/**
 * Function that allows to create the photo response by Atreides frame containing the basic info of the picture.
 * @param filename String containing the name of the picture to send.
 * @param size Integer containing the size of the picture to send.
 * @param MD5SUM String containing the MD5SUM of the picture to send.
 * @return Pointer to the frame string.
 */
char *tramaPhotoPicture(char *filename, int size, char *MD5SUM) {
    char *trama;
    char *dades = (char *) malloc(sizeof(char) * (strlen(filename) + sizeof(size) + strlen(MD5SUM) + 5));

    if (strlen(filename) > 30) return "1";

    trama = createOrigin("ATREIDES");
    trama[15] = 'F';

    sprintf(dades, "%s*%d*%s", filename, size, MD5SUM);

    trama = completeDataTrama(trama, dades);

    free(dades);

    return trama;
}


/**
 * Function that allows to create the photo request by Fremen frame.
 * @param id Integer containing the id of the user whose picture is requested to be sent.
 * @return Pointer to the frame string.
 */
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


/**
 * Function that allows to fill the data section of a frame with the picture data.
 * @param trama Pointer to the frame string.
 * @param buffer String containing the picture info.
 * @return Pointer to the frame string.
 */
char *completeDataPhoto(char *trama, char buffer[240]) {

    for (int i = 0; i < 240; i++) {
        trama[i + 16] = buffer[i];
    }

    return trama;
}


/**
 * Function that allows to create a frame containing photo info from Atreides.
 * @param photo_fd File descriptor of the picture to send.
 * @return Pointer to the frame string.
 */
char *sendDataPhotoAtreides(int photo_fd) {
    char *trama;

    trama = createOrigin("ATREIDES");
    trama[15] = 'D';

    char buffer[240];

    memset(buffer, 0, 240); //clean string before use it to avoid sending junk.

    read(photo_fd, buffer, 240);

    trama = completeDataPhoto(trama, buffer);

    return trama;
}


/**
 * Function that allows to create a frame containing photo info from Fremen.
 * @param photo_fd File descriptor of the picture to send.
 * @return Pointer to the frame string.
 */
char *sendDataPhoto(int photo_fd) {
    char *trama;

    trama = createOrigin("FREMEN");
    trama[15] = 'D';

    char buffer[240];

    memset(buffer, 0, 240); //clean string before use it to avoid sending junk.

    read(photo_fd, buffer, 240);

    trama = completeDataPhoto(trama, buffer);

    return trama;
}


/**
 * Function that allows to create the response to photo received by Atreides frame.
 * @param value Integer containing the type of response (1 for correct, 2 for incorrect).
 * @return Pointer to the frame string.
 */
char *sendImageResponseAtreides(int value) {
    char *trama;
    char *buffer;

    trama = createOrigin("ATREIDES");
    if (value == 1) {
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

/**
 * Function that allows to create the md5 of a file.
 * @param pathFoto String containing the path to the picture.
 * @return Pointer to the md5 created.
 */
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

        strtok(md5Hash, " "); //Since the md5sum returns the hash and the name of the file we need to split it.

        return md5Hash;
    }


    return NULL;
}


/**
 * Function that allows to get the file size.
 * @param pathFoto String containing the path of the file.
 * @return Integer containing the file size.
 */
int GetSizeFile(char *pathFoto) {
    struct stat st;

    stat(pathFoto, &st);
    int size = st.st_size;

    return size;
}
