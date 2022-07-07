#ifndef PRACTICA_ATREIDES_H
#define PRACTICA_ATREIDES_H


#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "ServerInfo.h"
#include "File.h"
#include "Frame.h"
#include "UserManagement.h"
#include "Connection.h"
#include "Plotcreation.h"
#include <arpa/inet.h>


void RSI_SIGINT();

void add_user(User user);

void create_connection(int fd, User user, pthread_t thread);

void login_user(int fd, Frame frame);

void search_users(int fd, Frame frame);

void data_photo_receive(char *size2, int fd, char *MD5SUM);

User get_user_by_fd(int fd);

void read_info_photo_send(Frame frame, int fd);

void remove_open_connection(char *username);

void logout(Frame frame);

void send_user_photo(int fd, Frame frame);

void *run_thread(void *fd_client);


#endif
