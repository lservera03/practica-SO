#ifndef PRACTICA_USERMANAGEMENT_H
#define PRACTICA_USERMANAGEMENT_H


#define USERS_FILE_ROUTE "users.csv"


typedef struct {
    int id;
    char *username;
    char *postal_code;
} User;


User getUserByString(char *string);


#endif //PRACTICA_USERMANAGEMENT_H
