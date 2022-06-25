#include "Atreides.h"

#define printF(x) write(1, x, strlen(x))

ServerInfo *serverInfo;
int listenFD, num_connections;
Connection *open_connections;
Users *users;


void RSI_SIGINT(){

	
	//TODO here free all memory and try to keep system stable

	

	//save users
	writeUsers(users);
	
	//Close socket
	close(listenFD);


	//set default RSI for SIGINT
	signal(SIGINT, SIG_DFL);
	//send SIGINT and let the default RSI handle it
	raise(SIGINT);

}



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


void add_user(User user){
	

	//Make space for the new user
	users->registered_users = (User *) realloc(users->registered_users, (((users->last_id + 1)) * sizeof(User)));


	//Save new user
	users->registered_users[users->last_id].id = user.id;
	strcpy(users->registered_users[users->last_id].postal_code, user.postal_code);
	users->registered_users[users->last_id].username = (char *) malloc(sizeof(char) * strlen(user.username));
	strcpy(users->registered_users[users->last_id].username, user.username);

	//Add +1 to the user counter
	users->last_id = (users->last_id + 1);
	
}


void login_user(Frame frame) {
    int exit = 0, found = 0, i = 0, id_user;
	//char *trama;
	char string_output[100];
	User user;
	char *parameters[2];

    //split frame.data to get username
    char *p = strtok (frame.data, "*");
    
   	while (p != NULL){
       	parameters[i++] = p;
       	p = strtok (NULL, "*");
   	}
			

	sprintf(string_output, "Rebut login %s %s\n", parameters[0], parameters[1]);

	printF(string_output);

    //Check if the user is already registered
    for (int i = 0; i < users->last_id && !exit; i++) {
        if (strcmp(users->registered_users[i].username, parameters[0]) == 0) { //The user exists

			id_user = users->registered_users[i].id;

            exit = 1;
            found = 1;
        }
    }

    if (found == 0) { //The user is new

		
		//create new user
		user.id = (users->last_id + 1);

		id_user = user.id;

		strcpy(user.postal_code, parameters[1]);
		user.username = (char *) malloc(sizeof(char) * strlen(parameters[0]));
		strcpy(user.username, parameters[0]);


		add_user(user);

    }


	sprintf(string_output, "Assignat a ID %d\n", id_user);

	printF(string_output);

	//TODO send response

	//trama = tramaConnectionCreated(users->registered_users[i].id);

}


void *run_thread(void *fd_client) {
    int fd = *((int *) fd_client);
    char frame_string[256];
	Frame frame;

    read(fd, frame_string, (sizeof(char) * 256));


    frame = createFrameFromString(frame_string);

    switch (frame.type) {
        case 'C': //LOGIN
            login_user(frame);
            break;
    }


    
	return NULL;
}

int main(int argc, char *argv[]) {
    int clientFD;
    int correct;
    struct sockaddr_in servidor;


    if (argc == 2) {
		
		//Assign SIGINT management to our function
		signal(SIGINT, (void *) RSI_SIGINT);

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

			
			//read users
			users = (Users *) malloc(sizeof(Users));
		    readUsers(users);


            while (1) {
                printF("Esperant connexions...\n");
                clientFD = accept(listenFD, (struct sockaddr *) NULL, NULL);
                if (clientFD > 0) {
                    //Create thread for every client
                    pthread_t threadClient;
                    pthread_create(&threadClient, NULL, run_thread, &clientFD);

                } else {
                    printF("ERROR on accept");
                    return -1;
                }


            }

        }

    } else if (argc < 2) {
        write(STDERR_FILENO, "NO has introduit els arguments necessaris\n",
              sizeof(char) * strlen("NO has introduit els arguments necessaris\n"));
    } else {
        write(STDERR_FILENO, "Has introduit massa arguments\n",
              sizeof(char) * strlen("Has introduit massa arguments\n"));
    }

	
	raise(SIGINT);

    return 0;
}
