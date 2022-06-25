#ifndef PRACTICA_USERMANAGEMENT_H
#define PRACTICA_USERMANAGEMENT_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define USERS_FILE_ROUTE "users.csv"
#define POSTAL_CODE_LENGTH 5

typedef struct {
    int id;
    char *username;
    char postal_code[POSTAL_CODE_LENGTH];
} User;


typedef struct {
    int last_id;
    User *registered_users;
} Users;


void readUsers(Users *users);

void writeUsers(Users *users);



#endif //PRACTICA_USERMANAGEMENT_H
