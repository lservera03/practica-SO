#include "UserManagement.h"


User getUserByString(char *username) {
    int fd, i, positions, final, found = 0;
    char character;
    char *buffer = NULL;
    User user;

    i = 0;
    positions = 1;

    fd = open(USERS_FILE_ROUTE, O_RDONLY, 0666);

    if (fd >= 0) {

        do {
            final = read(fd, &character, sizeof(char));

            buffer = (char *) realloc(buffer, i + 1);

            if (character == ',' || character == '\n' || final == 0) {

                buffer[i] = '\0';

                switch (positions) {
                    case 1:
                        user.id = atoi(buffer);
                        break;
                    case 2:
                        strcpy(user.username, buffer);
                        break;
                    case 3:
                        strcpy(user.postal_code, buffer);
                        break;
                }

                positions++;

                //Si em trobat l'usuari, el retornem
                if (strcmp(user.username, username) == 0) {
                    return user;
                }


                i = 0;
            } else {
                buffer[i] = character;

                i++;
            }

        } while (final > 0);


        close(fd);
        free(buffer);
    } else {
        write(STDERR_FILENO, "NO s'ha pogut obrir el fitxer d'usuaris\n",
              sizeof(char) * strlen("NO s'ha pogut obrir el fitxer\n"));
    }


    return NULL;
}

