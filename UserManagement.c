#include "UserManagement.h"

#define printF(x) write(1, x, strlen(x))


void readUsers(Users *users) {
    int fd, i, positions, final, num_users = 0, counter = 0;
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

                if (num_users == 0) {
                    num_users = atoi(buffer);

                    users->last_id = num_users;


                    users->registered_users = (User *) malloc(sizeof(User) * num_users);


                } else {
                    switch (positions) {
                        case 1:
                            user.id = atoi(buffer);
                            break;
                        case 2:
                            user.username = (char *) malloc(sizeof(char) * strlen(buffer) + 1);
                            strcpy(user.username, buffer);
                            break;
                        case 3:
                            strcpy(user.postal_code, buffer);
                            break;
                    }


                    positions++;
                }


                if (positions == 4) {
                    //Save new user

                    users->registered_users[counter].id = user.id;
                    strcpy(users->registered_users[counter].postal_code, user.postal_code);
					users->registered_users[counter].username = (char *) malloc(sizeof(char) * strlen(user.username) + 1);
                    strcpy(users->registered_users[counter].username, user.username);


                    counter++;
                    positions = 1;
					free(user.username);
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

        users->last_id = 0;

        write(STDERR_FILENO, "NO s'ha pogut obrir el fitxer d'usuaris\n",
              sizeof(char) * strlen("NO s'ha pogut obrir el fitxer d'usuaris\n"));
    }


}



void writeUsers(Users *users){
	char write_string[100];
    int write_fd;

    //Open file
    write_fd = open(USERS_FILE_ROUTE, O_CREAT | O_RDWR, 0666);

    if (write_fd < 0) {
        printF("NO s'ha pogut obrir el fitxer d'usuaris\n");
    } else {

        //Write last id given
        sprintf(write_string, "%d\n", users->last_id);
        write(write_fd, write_string, sizeof(char) * strlen(write_string));

		//Write every user into the file
        for (int i = 0; i < users->last_id; i++) {
            sprintf(write_string, "%d,%s,%s\n", users->registered_users[i].id, users->registered_users[i].username, users->registered_users[i].postal_code);
            write(write_fd, write_string, sizeof(char) * strlen(write_string));
        }

        close(write_fd);
    }
	

}



