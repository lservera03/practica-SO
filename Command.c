#include "Command.h"



Command *command;

int executeCommand(char string[]) {
    char upper[50];

    int i = 0, exit;

    exit = 0;

    createCommand(string);

    //Convert command to upper case
    while (string[i] != ' ' && string[i] != '\n' && string[i] != '\0') {
        upper[i] = toupper(string[i]);
        i++;
    }

    upper[i] = '\0';

    if (strcmp(upper, "LOGIN") == 0) {
        if (command->num_arguments == 3) {
            //TODO check if the order of the arguments is correct
            //if () {
            //  write(STDOUT_FILENO, "Comanda KO. Parametres incorrectes\n",
            //      sizeof(char) * strlen("Comanda KO. Parametres incorrectes\n"));
            //} else {

            /*
             ServerInfo serverinfo;
    struct sockaddr_in s_addr;
    int fd;

    bzero(config.IP, sizeof(config.IP));
    config.port = -1;

    if(!readConfig(&config)){
        print("ERROR: fichero de configuracion erroneo o inexistente\n");
        exit(EXIT_FAILURE);
    }

    fd = socket(AF_INET,SOCK_STREAM, 0);
    if(fd < 0){
        print("ERROR: crear socket del cliente\n");
        exit(-1);
    }

    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(config.port);
    s_addr.sin_addr.s_addr = inet_addr(config.IP);

    if (connect(fd, (void *)&s_addr, sizeof(s_addr)) < 0){
        print("ERROR: connect del cliente\n");
        close(fd);
        exit(-1);
    }
             */

            write(STDOUT_FILENO, "Comanda OK\n", sizeof(char) * strlen("Comanda OK\n"));
            //}
        } else {
            write(STDOUT_FILENO, "Comanda KO. Falta paràmetres\n",
                  sizeof(char) * strlen("Comanda KO. Falta paràmetres\n"));
        }
    } else if (strcmp(upper, "SEARCH") == 0) {
        if (command->num_arguments == 2) {
            write(STDOUT_FILENO, "Comanda OK\n", sizeof(char) * strlen("Comanda OK\n"));
        } else {
            write(STDOUT_FILENO, "Comanda KO. Falta paràmetres\n",
                  sizeof(char) * strlen("Comanda KO. Falta paràmetres\n"));
        }
    } else if (strcmp(upper, "SEND") == 0) {
        if (command->num_arguments == 2) {
            write(STDOUT_FILENO, "Comanda OK\n", sizeof(char) * strlen("Comanda OK\n"));
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
        } else {
            write(STDOUT_FILENO, "Comanda KO. Falta paràmetres\n",
                  sizeof(char) * strlen("Comanda KO. Falta paràmetres\n"));
        }
    } else if (strcmp(upper, "LOGOUT") == 0) {
        if (command->num_arguments == 1) {
            write(STDOUT_FILENO, "Comanda OK\n", sizeof(char) * strlen("Comanda OK\n"));

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
        } else if (fill == 0) {


            char *args[command->num_arguments + 1];

            for (int j = 0; j < command->num_arguments; j++) {
                args[j] = command->arguments[j];
            }

            args[command->num_arguments] = NULL;

            //TODO check if command exists
            execvp(args[0], args);


            freeMemoryCommand();
            command->num_arguments = -1;


            return -1;
        } else {
            write(1, "ERROR al crear el fork\n", sizeof("ERROR al crear el fork\n"));
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

        counter++;;
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




