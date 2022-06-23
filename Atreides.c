#include "Atreides.h"

#define printF(x) write(1, x, strlen(x))

ServerInfo *serverInfo;
int listenFD;
int openSockets[50];
int contSockets;
pthread_t threads[50];

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


void login_user(int fd, Frame frame) {

    //TODO split frame.data to get username

    //TODO check if the user is already registered

    //TODO If not, register user

    //TODO write response

}


void *run_thread(void *fd_client) {
    int fd = *((int *) fd_client);
    char frame_string[256];
    Frame frame;

    read(fd, frame_string, (sizeof(char) * 256));

    frame = createFrameFromString(frame_string);

    switch (frame.type) {
        case 'C': //LOGIN
            login_user(fd, frame);
            break;
    }


    return NULL;
}

int main(int argc, char *argv[]) {
    int correct;
    struct sockaddr_in servidor;


    if (argc == 2) {
        contSockets = 0;

        serverInfo = (ServerInfo *) malloc(sizeof(ServerInfo));

        write(STDOUT_FILENO, "SERVIDOR ATREIDES\n",
              sizeof(char) * strlen("SERVIDOR ATREIDES\n"));

        correct = readConfigFile(argv[1], serverInfo, ATREIDES_CONFIG);

        if (correct) {
            write(STDOUT_FILENO, "Llegit el fitxer de configuració\n",
                  sizeof(char) * strlen("Llegit el fitxer de configuració\n"));

            if ((listenFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                printF("Error creant el socket\n");
            }

            bzero(&servidor, sizeof(servidor));
            servidor.sin_port = htons(8700); //Falla al posar-ho serverInfo-port
            servidor.sin_family = AF_INET;
            servidor.sin_addr.s_addr = htons(INADDR_ANY);

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
        write(STDERR_FILENO, "NO has introduit els arguments necessaris\n",
              sizeof(char) * strlen("NO has introduit els arguments necessaris\n"));
    } else {
        write(STDERR_FILENO, "Has introduit massa arguments\n",
              sizeof(char) * strlen("Has introduit massa arguments\n"));
    }


}