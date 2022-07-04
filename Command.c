#include "Command.h"
#include "stdio.h"
#include "unistd.h"

#define printF(x) write(1, x, strlen(x))


Command *command;
int atreides_fd, is_logged = 0, id_user = -1;
ServerInfo *server_info;
char *username;

void RSI_SIGINT() {


    if (is_logged) { //Send logout if the user is logged in
        char *trama;

        trama = tramaFinishConeixion(username, id_user);

        write(atreides_fd, trama, 256);

        printF("Desconnectat d'Atreides. Dew!\n");

        free(trama);
    }

    free(username);

	if(command != NULL){
		freeMemoryCommand();
	}

    close(atreides_fd);



}


void create_connection_atreides() {
    struct sockaddr_in s_addr;

    //Create socket
    atreides_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (atreides_fd < 0) {
        printF("ERROR: NO s'ha pogut crear el socket\n");
    }

    //Config socket
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(server_info->port);
    s_addr.sin_addr.s_addr = inet_addr(server_info->ip);

    //Connect to Atreides by the socket created
    if (connect(atreides_fd, (void *) &s_addr, sizeof(s_addr)) < 0) {
        printF("ERROR: NO s'ha pogut connectar al socket\n");
        close(atreides_fd);
        atreides_fd = -1;
    }

}

int executeCommand(char string[], ServerInfo *serverInfo) {
    char upper[50];
    char *frame;
    char frame_string[256];
    char string_output[100];
    Frame frame_struct;

    int i = 0, exit, size;
    int number_frame;

    exit = 0;

    server_info = serverInfo;

    createCommand(string);

    //Convert command to upper case
    while (string[i] != ' ' && string[i] != '\n' && string[i] != '\0') {
        upper[i] = toupper(string[i]);
        i++;
    }

    upper[i] = '\0';

    if (upper[0] != '\0') {


        if (strcmp(upper, "LOGIN") == 0) {
            if (command->num_arguments == 3) {

                if (!is_logged) {

                    char *ptr = command->arguments[2];

                    strtol(ptr, &ptr, 10);

                    if (*ptr == '\0') {

                        if (*ptr == '\0') {

                            create_connection_atreides();

                            //Check if the socket is correct
                            if (atreides_fd != -1) {

                                username = (char *) malloc(sizeof(char) * strlen(command->arguments[1]) + 1);

                                strcpy(username, command->arguments[1]);

                                frame = tramaStartConexion(command->arguments[1], command->arguments[2]);

                                //Enviar trama solicitant connexió
                                write(atreides_fd, frame, 256);

                                //Wait for response
                                read(atreides_fd, frame_string, (sizeof(char) * 256));

                                //Process response received
                                frame_struct = createFrameFromString(frame_string);


                                id_user = atoi(frame_struct.data);

                                sprintf(string_output, "Benvingut %s. Tens ID %d\n", username, id_user);

                                printF(string_output);

                                printF("Ara estàs connectat a Atreides.\n");

                                is_logged = 1;

								free(frame);
                            } else {
                                printF("ERROR: NO s'ha pogut connectar amb Atreides\n");
                            }
                        }

                    } else {
                        printF("ERROR: El codi postal ha de ser un numero\n");
                    }
                } else {
                    printF("Ja hi ha un usuari loguejat\n");
                }
            } else {
                write(STDOUT_FILENO, "Comanda KO. Falta paràmetres\n",
                      sizeof(char) * strlen("Comanda KO. Falta paràmetres\n"));
            }
        } else if (strcmp(upper, "SEARCH") == 0) {
            if (command->num_arguments == 2) {

                //Check if the user is logged in
                if (is_logged) {

                    char *ptr = command->arguments[1];

                    strtol(ptr, &ptr, 10);

                    if (*ptr == '\0') {

                        frame = tramaSearch(username, id_user, command->arguments[1]);

                        //Send frame requesting the search
                        write(atreides_fd, frame, 256);

                        //Wait response with the users found
                        read(atreides_fd, frame_string, (sizeof(char) * 256));


                        frame_struct = createFrameFromString(frame_string);


                        //If there are no users found
                        if (strlen(frame_struct.data) == 1) {
                            sprintf(string_output, "No hi ha cap persona humana a %s\n", command->arguments[1]);
                            printF(string_output);
                        } else { //If there are users

                            //Split info and show
                            int name = 1, first = 1;
                            int num_users_found = -1;
                            char aux[60];
                            int end = 0, counter = 0;

                            char *p = strtok(frame_struct.data, "*");

                            num_users_found = atoi(p);

                            memset(string_output, '\0', 256);

                            sprintf(string_output, "Hi han %d persones humanes a %s\n", num_users_found,
                                    command->arguments[1]);

                            printF(string_output);


                            while (!end) { //Loop to read frames until we have printed all the users

                                if (first) {
                                    p = strtok(NULL, "*");
                                    first = 0;
                                }

                                while (p != NULL) {
                                    if (name) {
                                        name = 0;
                                        strcpy(aux, p);
                                    } else {
                                        name = 1;
                                        sprintf(string_output, "%s %s\n", p, aux);
                                        printF(string_output);
                                        counter++;
                                    }

                                    p = strtok(NULL, "*");
                                }

                                if (counter == num_users_found) {
                                    end = 1;
                                } else {
                                    read(atreides_fd, frame_string, sizeof(char) * 256);

                                    frame_struct = createFrameFromString(frame_string);

                                    p = strtok(frame_struct.data, "*");

                                    name = 1;
                                }
                            }

                        }

						free(frame);
                    } else {
                        printF("ERROR: El id ha de ser un numero\n");
                    }

                } else {
                    printF("Has d'estar loguejat per executar aquesta comanda!\n");
                }

            } else {
                write(STDOUT_FILENO, "Comanda KO. Falta paràmetres\n",
                      sizeof(char) * strlen("Comanda KO. Falta paràmetres\n"));
            }
        } else if (strcmp(upper, "SEND") == 0) {

            //Check if the user is logged in
            if (is_logged) {

                if (command->num_arguments == 2) {

                    char *pathFoto = (char *) malloc(
                            sizeof(char) * (strlen(serverInfo->directory) + 1 + strlen(command->arguments[1]) + 2));

                    sprintf(pathFoto, ".%s/%s", serverInfo->directory, command->arguments[1]);

                    if (access(pathFoto, F_OK) != 0) {
                        printF("Error: La imagen no existe\n");

                    } else {
                        size = GetSizeFile(pathFoto);

						char *generated_md5 = MD5Generate(pathFoto);

                        frame = tramaSearchPicture(command->arguments[1], size, generated_md5);

						
						free(generated_md5);
					
                        if (strcmp(frame, "1") == 0) {
                            write(STDOUT_FILENO, "ERROR: filename too big\n",
                                  sizeof(char) * strlen("ERROR: filename too big\n"));
                        } else {


                            write(atreides_fd, frame, 256);

                            int photo_fd = open(pathFoto, O_RDONLY);

                            free(pathFoto);
							free(frame);

                            if ((size % 240) != 0) {
                                number_frame = (size / 240) + 1;
                            } else {
                                number_frame = (size / 240);
                            }

                            for (int z = 0; z < number_frame; z++) {

                                frame = sendDataPhoto(photo_fd);
                                write(atreides_fd, frame, 256);

								free(frame);

                            }
                            close(photo_fd);

                            read(atreides_fd, frame_string, 256);
                            frame_struct = createFrameFromString(frame_string);

                            switch (frame_struct.type) {
                                case 'I':
                                    printF("Foto enviada amb exit Atreides\n");
                                    break;
                                case 'R':
                                    printF("ERROR: Al enviar la foto a Atreides\n");
                                    break;

                            }
                        }

                    }

                } else if (command->num_arguments < 2) {
                    write(STDOUT_FILENO, "Comanda KO. Falta paràmetres\n",
                          sizeof(char) * strlen("Comanda KO. Falta paràmetres\n"));
                } else {
                    write(STDOUT_FILENO, "Comanda KO. Massa paràmetres\n",
                          sizeof(char) * strlen("Comanda KO. Massa paràmetres\n"));
                }

            } else {
                printF("Has d'estar loguejat per executar aquesta comanda!\n");
            }

        } else if (strcmp(upper, "PHOTO") == 0) {
            if (command->num_arguments == 2) {

                if (is_logged) {

                    char *ptr = command->arguments[1];

                    strtol(ptr, &ptr, 10);

                    if (*ptr == '\0') {

                        frame = tramaPhotoRequest(command->arguments[1]);

                        //Send frame requesting the picture
                        write(atreides_fd, frame, 256);

						free(frame);

                        //Wait for response from Atreides
                        read(atreides_fd, frame_string, sizeof(char) * 256);


                        frame_struct = createFrameFromString(frame_string);


                        //check if the photo is found
                        if (strcmp(frame_struct.data, "FILE NOT FOUND") == 0) {
                            printF("No hi ha foto registrada.\n");
                        } else {


                            //Split frame.data to get photo info
                            char *p = strtok(frame_struct.data, "*");
                            char *parameters[3];
                            char *pathFoto;
                            char originalMD5[32], filename[30];
                            int i = 0, size, num_frames, file, last_frame_short = 0;


                            while (p != NULL) {
                                parameters[i++] = p;
                                p = strtok(NULL, "*");
                            }

                            size = atoi(parameters[1]);

                            strcpy(originalMD5, parameters[2]);


                            strcpy(filename, parameters[0]);


                            //calculate number of frames
                            num_frames = size / 240;

                            if ((num_frames % 240) != 0) {
                                num_frames++;
                                last_frame_short = 1;
                            }

                            //create path of the picture
                            pathFoto = (char *) malloc(
                                    sizeof(char) * (strlen(serverInfo->directory) + strlen(filename) + 2));

                            sprintf(pathFoto, ".%s/%s", serverInfo->directory, filename);


                            //open file to save picture
                            file = open(pathFoto, O_CREAT | O_WRONLY | O_TRUNC, 0666);


                            //read photo frame by frame
                            for (int i = 0; i < num_frames; i++) {

                                read(atreides_fd, frame_string, 256);

                                frame_struct = createFrameFromString(frame_string);


                                if (i == (num_frames - 1) && last_frame_short) { //LAST FRAME
                                    write(file, frame_struct.data, size % 240);
                                } else {
                                    write(file, frame_struct.data, 240);
                                }

                                memset(frame_string, 0, strlen(frame_string));

                            }

                            close(file);

							char *generated_md5 = MD5Generate(pathFoto);

                            //check MD5SUM is correct
                            if (strcmp(generated_md5, originalMD5) == 0) {

                                printF("Foto descarregada\n");

                                memset(frame, 0, strlen(frame));

                                //send  photo frame correct
                                frame = tramaPhotoCorrect();

                                write(atreides_fd, frame, 256);
                            } else {
                                printF("Ha ocorregut un error durant la descarrega de la foto\n");

                                //send incorrect photo frame

                                memset(frame, 0, strlen(frame));

                                frame = tramaPhotoNotCorrect();

                                write(atreides_fd, frame, 256);
                            }

                            memset(filename, '\0', strlen(filename));

							free(generated_md5);
                            free(p);
                            free(pathFoto);
							free(frame);
                        }

                    } else {
                        printF("ERROR: El id de la foto no es un numero\n");
                    }


                } else {
                    printF("Has d'estar loguejat per executar aquesta comanda!\n");
                }

            } else {
                write(STDOUT_FILENO, "Comanda KO. Falta paràmetres\n",
                      sizeof(char) * strlen("Comanda KO. Falta paràmetres\n"));
            }
        } else if (strcmp(upper, "LOGOUT") == 0) {
            if (command->num_arguments == 1) {

                //CHeck if the user is logged in
                if (is_logged) {


                    frame = tramaFinishConeixion(username, id_user);

                    //send logout request
                    write(atreides_fd, frame, 256);


                    //Close open socket
                    close(atreides_fd);

                    printF("Desconnectat d’Atreides. Dew!\n");

                    is_logged = 0;

                   	exit = 1;

					free(frame);
                } else {
                    printF("Has d'estar loguejat per executar aquesta comanda!\n");
                }

            } else {
                write(STDOUT_FILENO, "Comanda KO. Massa paràmetres\n",
                      sizeof(char) * strlen("Comanda KO. Massa paràmetres\n"));
            }
        } else { // Comando por defecto de Linux
            int fill; //pid;



            //Fork para ejecutar el comando de Linux
            fill = fork();
            if (fill > 0) {
                //pid = getpid();
                wait(NULL);
                freeMemoryCommand();
           	} else if (fill == 0) {


                char *args[command->num_arguments + 1];

                for (int j = 0; j < command->num_arguments; j++) {
                    args[j] = command->arguments[j];
                }

                args[command->num_arguments] = NULL;

                //Execute command and check if it's correct
                if (execvp(args[0], args) < 0) {
                    write(1, "ERROR al ejecutar el comando\n", sizeof(char) * strlen("ERROR al ejecutar el comando\n"));
					_exit(1); //necessary because when execvp fails the process does not finish
               	}


                //freeMemoryCommand();

                //command->num_arguments = -1;
			

               // return 1;
            } else {

                freeMemoryCommand();
                write(1, "ERROR al crear el fork\n", sizeof("ERROR al crear el fork\n"));

            }
        }
		
		freeMemoryCommand();
    }

	

    return exit;
}


void removeNewLine(char *string) {
    char character;
    int i = 0;

    do {
        character = string[i];
        if (character == '\n') {
            string[i] = '\0';
        }
        i++;
    } while (character != '\n' && character != '\0');

}


int countArguments(char string[]) {
    char *copy, *splited;
    int counter = 0;

    copy = (char *) malloc(strlen(string) + 1);
    strcpy(copy, string);

    splited = strtok(copy, " ");

    while (splited != NULL) {
        splited = strtok(NULL, " ");

        counter++;
    }

	free(copy);

    return counter;
}


void createCommand(char string[]) {
    char *splited, *copy;
    int counter = 0, end = 0, arguments;

    command = (Command *) malloc(sizeof(Command));

    copy = (char *) malloc(strlen(string) + 1);
    strcpy(copy, string);

    arguments = countArguments(string);

    command->arguments = (char **) malloc(arguments * sizeof(char *));

    splited = strtok(copy, " ");

    do {
        removeNewLine(splited);
        command->arguments[counter] = (char *) malloc(sizeof(char) * (strlen(splited) + 1));
        strcpy(command->arguments[counter], splited);
        command->arguments[counter][strlen(splited)] = '\0';

        counter++;

        splited[0] = '\0';
        splited = strtok(NULL, " ");

        if (splited == NULL) {
            end = 1;
        }

    } while (!end);

    command->num_arguments = arguments;

	free(copy);
}


void freeMemoryCommand() {

    if (command != NULL) {
        for (int j = 0; j < command->num_arguments; j++) {
            if (command->arguments[j] != NULL) {
                free(command->arguments[j]);
            }
        }
        if (command->arguments != NULL) {
            free(command->arguments);
        }

        free(command);
    }

	command = NULL;

}


int checkIfIsNumber(char *string[]) {
    char character;
    int i = 0;

    printF(*string);

    do {
        character = *string[i];
        if (character < 48 || character > 57) {
            return 0;
        }
        i++;
    } while (character != '\n' && character != '\0');


    return 1;
}





