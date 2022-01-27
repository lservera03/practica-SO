#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ServerInfo.h"
#include "File.h"

#define printF(x) write(1, x, strlen(x))

ServerInfo *serverInfo;
int listenFD;

char *readLine(int fd, char delimiter) {
    char *msg = (char *) malloc(1);
    char current;
    int i = 0;
    int len = 0;

    while ((len += read(fd, &current, 1)) > 0) {
        msg[i] = current;
        msg = (char *) realloc(msg, ++i + 1);
        if (current == delimiter) {
            break;
        }
    }
    msg[i - 1] = '\0';


    return msg;
}


int main(int argc, char *argv[]) {
    int correct;
    struct sockaddr_in servidor;


    if (argc == 2) {
        serverInfo = (ServerInfo *) malloc(sizeof(ServerInfo));

        correct = readConfigFile(argv[1], serverInfo, ATREIDES_CONFIG);

        if (correct) {
            write(STDOUT_FILENO, "Llegit el fitxer de configuració\n",
                  sizeof(char) * strlen("Llegit el fitxer de configuració\n"));

            if ((listenFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
                printF("Error creant el socket\n");
            }

            bzero(&servidor, sizeof(servidor));
            servidor.sin_port = htons(serverInfo->port);
            servidor.sin_family = AF_INET;
            servidor.sin_addr.s_addr = htonl(INADDR_ANY);

            if (bind(listenFD, (struct sockaddr *) &servidor, sizeof(servidor)) < 0) {
                printF("Error fent el bind\n");
            }

            if (listen(listenFD, 10) < 0) {
                printF("Error fent el listen\n");
            }

            while (1) {




            }

        }

    } else if (argc < 2) {
        write(STDERR_FILENO, "NO has introduit els arguments necessaris",
              sizeof(char) * strlen("NO has introduit els arguments necessaris"));
    } else {
        write(STDERR_FILENO, "Has introduit massa arguments",
              sizeof(char) * strlen("Has introduit massa arguments"));
    }


}