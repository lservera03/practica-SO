#include "Command.h"
#include "stdio.h"
#include "unistd.h"
#define printF(x) write(1, x, strlen(x))


Command *command;
int atreides_fd, is_logged = 0, id_user = -1;
ServerInfo *server_info;
char *username;


//TODO RSI_SIGINT

void RSI_SIGINT(){


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
    //TODO CAMBIAR id SOCKET
    char *id = NULL;
    // todo poner valor a size del fichero y MD5SUM
   // char *size = NULL;
   // char *MD5SUM = NULL;
    //char *dadesBinarias ;
    char *trama;
	char frame_string[256];
	char string_output[100];
	Frame frame_struct;

    int i = 0, exit;

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

                char *ptr = command->arguments[2];

                strtol(ptr, &ptr, 10);

                if (*ptr == '\0') {

                    write(STDOUT_FILENO, "Comanda OK\n", sizeof(char) * strlen("Comanda OK\n"));

                    if (*ptr == '\0') {
                        //TODO check if there is already a user logged in.


                        create_connection_atreides();

                        //Check if the socket is correct
                        if (atreides_fd != -1) {

							username = (char *) malloc(sizeof(char) * strlen(command->arguments[1]));

							strcpy(username, command->arguments[1]);

                            frame = tramaStartConexion(command->arguments[1], command->arguments[2]);

                            //Enviar trama solicitant connexió
                            write(atreides_fd, frame, 256);

							//Wait for response
							read(atreides_fd, frame_string, (sizeof(char) * 256));

							//Process response received
							frame_struct = createFrameFromString(frame_string);


							//TODO check if the frame is correct(O) or not (E)
							
							id_user = atoi(frame_struct.data);

							sprintf(string_output, "Benvingut %s. Tens ID %d\n", username, id_user);

							printF(string_output);

							printF("Ara estàs connectat a Atreides.");

							is_logged = 1;



                        } else {
                            printF("ERROR: NO s'ha pogut connectar amb Atreides\n");
                        }
                    }

                } else {
                    printF("ERROR: El codi postal ha de ser un numero\n");
                }

            } else {
                write(STDOUT_FILENO, "Comanda KO. Falta paràmetres\n",
                      sizeof(char) * strlen("Comanda KO. Falta paràmetres\n"));
            }
        } else if (strcmp(upper, "SEARCH") == 0) {
            if (command->num_arguments == 2) {

				//Check if the user is logged in
				if(is_logged){

					trama = tramaSearch(username, id_user, command->arguments[1]);

					//Send frame requesting the search
					write(atreides_fd, trama, 256);




				} else {
					printF("You must be logged in to execute that command!\n");
				}


            } else {
                write(STDOUT_FILENO, "Comanda KO. Falta paràmetres\n",
                      sizeof(char) * strlen("Comanda KO. Falta paràmetres\n"));
            }
        } else if (strcmp(upper, "SEND") == 0) {
            if (command->num_arguments == 2) {
             //   write(STDOUT_FILENO, "Comanda OK\n", sizeof(char) * strlen("Comanda OK\n"));

                // todo liberar memoria
                char *pathFoto = (char *) malloc(sizeof(strlen("fremen1/")) + sizeof(command->arguments[1]));

                sprintf(pathFoto, "fremen1/%s",command->arguments[1]);

              //  int z = open(pathFoto,O_RDONLY);

                if (access(pathFoto , F_OK) != 0) {
                    printF("Error: La imagen no existe\n");
                } else {

                    // frame = tramaSearchPicture(command->arguments[1], size, MD5SUM);
                    MD5Generate(pathFoto);
                    // frame = sendDataPhoto(dadesBinarias);

                   /* if (strcmp(frame, "1") == 0) {
                        write(STDOUT_FILENO, "ERROR: filename too big\n",
                              sizeof(char) * strlen("ERROR: filename too big\n"));
                    }
                    if (strcmp(frame, "2") == 0) {
                        write(STDOUT_FILENO, "ERROR: MD5SUM too big\n", sizeof(char) * strlen("ERROR: MD5SUM too big\n"));
                    }*/
                }//gcc -c Plotcreation.c -Wall -Wextra

            } else if (command->num_arguments < 2) {
                write(STDOUT_FILENO, "Comanda KO. Falta paràmetres\n",
                      sizeof(char) * strlen("Comanda KO. Falta paràmetres\n"));
            } else {
                write(STDOUT_FILENO, "Comanda KO. Massa paràmetres\n",
                      sizeof(char) * strlen("Comanda KO. Massa paràmetres\n"));
            }
        } else if (strcmp(upper, "PHOTO") == 0) {
            if (command->num_arguments == 2) {
                write(STDOUT_FILENO, "Comanda OK\n", sizeof(char) * strlen("Comanda OK\n"));
                trama = tramaPhotoPeticion(command->arguments[1]);


                 frame = tramaPhotoPeticion(command->arguments[1]);
            } else {
                write(STDOUT_FILENO, "Comanda KO. Falta paràmetres\n",
                      sizeof(char) * strlen("Comanda KO. Falta paràmetres\n"));
            }
        } else if (strcmp(upper, "LOGOUT") == 0) {
            if (command->num_arguments == 1) {
                write(STDOUT_FILENO, "Comanda OK\n", sizeof(char) * strlen("Comanda OK\n"));
                trama = tramaFinishConeixion(command->arguments[1], id);
                exit = 1;
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
                    write(1, "ERROR al ejecutar el comando\n", sizeof("ERROR al ejecutar el comando\n"));

                }
                /** free memory when wrong comand*/
                /*
                 freeMemoryCommand();*/

                command->num_arguments = -1;


                return -1;
            } else {

                freeMemoryCommand();
                write(1, "ERROR al crear el fork\n", sizeof("ERROR al crear el fork\n"));

            }
        }
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

    copy = (char *) malloc(strlen(string));
    strcpy(copy, string);

    splited = strtok(copy, " ");

    while (splited != NULL) {
        splited = strtok(NULL, " ");

        counter++;
    }

    return counter;
}


void createCommand(char string[]) {
    char *splited, *copy;
    int counter = 0, end = 0, arguments;

    command = (Command *) malloc(sizeof(Command));

    copy = (char *) malloc(strlen(string));
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





