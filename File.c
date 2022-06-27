#include "File.h"


int readConfigFile(char file[50], ServerInfo *serverInfo, int config) {
    int fd, i, positions, final, correct, first = 1;
    char character;
    char *buffer = NULL;

    i = 0;
    positions = 1;

    fd = open(file, O_RDONLY, 0666);

    if (fd >= 0) {

        correct = 1;

        do {
            final = read(fd, &character, sizeof(char));

            buffer = (char *) realloc(buffer, i + 1);

            if (character == '\n' || final == 0) {

                buffer[i] = '\0';

                if (config == ATREIDES_CONFIG && first == 1) { //In case we are reading config file of Atreides
                    positions++;
                    serverInfo->seconds = -1;
					first = 0;
                }

                switch (positions) {
                    case 1:
                        serverInfo->seconds = atoi(buffer);
                        break;
                    case 2:
                        strcpy(serverInfo->ip, buffer);
                        break;
                    case 3:
                        serverInfo->port = atoi(buffer);
                        break;
                    case 4:
                        strcpy(serverInfo->directory, buffer);
                        break;
                }

                positions++;

                i = 0;
            } else {
                buffer[i] = character;

                i++;
            }

        } while (final > 0);


        close(fd);
        free(buffer);
    } else {
        write(STDERR_FILENO, "NO s'ha pogut obrir el fitxer",
              sizeof(char) * strlen("NO s'ha pogut obrir el fitxer"));

        correct = 0;
    }

    return correct;
}
