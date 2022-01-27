#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <printf.h>

#include "ServerInfo.h"
#include "File.h"
#include "Command.h"


ServerInfo *serverInfo;

void end() {

    if (serverInfo != NULL) {
        free(serverInfo);
    }
    freeMemoryCommand();

    raise(SIGINT);
}


int main(int argc, char *argv[]) {
    int correct, i, exit;
    char character, *buffer;

    i = 0;
    buffer = NULL;
    character = ' ';

    signal(SIGINT, end);

    if (argc == 2) {

        serverInfo = (ServerInfo *) malloc(sizeof(ServerInfo));

        correct = readConfigFile(argv[1], serverInfo, FREMEN_CONFIG);
        if (correct) {

            write(STDOUT_FILENO, "Benvingut a Fremen\n", sizeof(char) * strlen("Benvingut a Fremen\n"));

            do {
                write(STDOUT_FILENO, "$ ", sizeof(char) * strlen("$ "));
                while (character != '\n') {
                    read(STDIN_FILENO, &character, sizeof(char));


                    buffer = (char *) realloc(buffer, i + 1);

                    buffer[i] = character;

                    i++;
                }

                buffer[i] = '\0';

                exit = executeCommand(buffer);

                i = 0;
                buffer[0] = '\0';
                character = ' ';


            } while (!exit);

        }

        free(buffer);
        free(serverInfo);

    } else if (argc < 2) {
        write(STDERR_FILENO, "NO has introduit els arguments necessaris",
              sizeof(char) * strlen("NO has introduit els arguments necessaris"));
    } else {
        write(STDERR_FILENO, "Has introduit massa arguments",
              sizeof(char) * strlen("Has introduit massa arguments"));
    }


    return 0;
}

