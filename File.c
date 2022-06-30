#include "File.h"


int readConfigFile(char file[50], ServerInfo *serverInfo, int config) {
    int fd, positions, final, correct;
    int i = 0, first = 1;
    char character;
    char *buffer = NULL;
	char *create_directory;


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


		//Create directory if not exists
        // todo he cambiado +1 --> +2 memoria valgrind arreglado
		create_directory = (char *) malloc(sizeof(char) * (strlen(serverInfo->directory) + 2));

		sprintf(create_directory, ".%s", serverInfo->directory);

		mkdir(create_directory, 0700);

        free(create_directory);

        close(fd);

    } else {
        write(STDERR_FILENO, "NO s'ha pogut obrir el fitxer",
              sizeof(char) * strlen("NO s'ha pogut obrir el fitxer"));

        correct = 0;
    }
    free(buffer);
    return correct;
}
