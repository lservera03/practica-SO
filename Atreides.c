#include <stdbool.h>
#include "Atreides.h"

#define printF(x) write(1, x, strlen(x))

ServerInfo *serverInfo;
int listenFD, num_connections;
Connection *open_connections;
Users *users;


void read_info_photo_send();

User get_user_by_fd(int fd);

char *read_all_frame_photo();

void RSI_SIGINT() {


    //TODO here free all memory and try to keep system stable



    //save users
    writeUsers(users);

    //Close socket
    close(listenFD);


	//TODO free connections memory

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


void add_user(User user) {


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


void create_connection(int fd, User user, pthread_t thread){

	//create space for new connection
	if(num_connections == 0){
		open_connections = (Connection *) malloc(sizeof(Connection));
	} else {
		open_connections = (Connection *) realloc(open_connections, (num_connections + 1) * sizeof(Connection));
	}

	//create new connection
	open_connections[num_connections].user.id = user.id;
	strcpy(open_connections[num_connections].user.postal_code, user.postal_code);
	open_connections[num_connections].user.username = (char *) malloc(sizeof(char) * strlen(user.username));
	strcpy(open_connections[num_connections].user.username, user.username);
	open_connections[num_connections].file_descriptor = fd;
	open_connections[num_connections].thread = thread;

	num_connections++;

}


void login_user(int fd, Frame frame) {
    int exit = 0, found = 0, i = 0, id_user;
    char *trama;
    char string_output[100];
    User user;
    char *parameters[2];

    //split frame.data to get username
    char *p = strtok(frame.data, "*");

    while (p != NULL) {
        parameters[i++] = p;
        p = strtok(NULL, "*");
    }

    sprintf(string_output, "Rebut login %s %s\n", parameters[0], parameters[1]);

    printF(string_output);

    //Check if the user is already registered
    for (int i = 0; i < users->last_id && !exit; i++) {
        if (strcmp(users->registered_users[i].username, parameters[0]) == 0) { //The user exists

            id_user = users->registered_users[i].id;

				
			create_connection(fd, users->registered_users[i], pthread_self());

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

		//create new connection
		create_connection(fd, user, pthread_self());

    }


    trama = tramaConnectionCreated(id_user);

    sprintf(string_output, "Assignat a ID %d\n", id_user);

    printF(string_output);

    //send response
    write(fd, trama, 256);

    printF("Enviada resposta\n\n");

}


void search_users(int fd, Frame frame) {
    char *parameters[3];
    char string_output[100];
    int i = 0, counter = 0;
    char data[240];
    char aux[100];
    char *trama;


    memset(aux, '\0', 100);
    memset(data, '\0', 240);

    //split frame.data to get parameters (postal_code)
    char *p = strtok(frame.data, "*");


    while (p != NULL) {
        parameters[i++] = p;
        p = strtok(NULL, "*");
    }

    sprintf(string_output, "Rebut search %s de %s %s\n", parameters[2], parameters[0], parameters[1]);

    printF(string_output);

    //Clean string output
    memset(string_output, '\0', strlen(string_output));

    //search users witht that postal code
    for (int j = 0; j < users->last_id; j++) {
        if (strcmp(users->registered_users[j].postal_code, parameters[2]) == 0) { //Find a user with that postal code
            counter++;
        }
    }

    printF("Feta la cerca\n");

    sprintf(string_output, "Hi ha %d persones humanes a %s\n", counter, parameters[2]);

    printF(string_output);


    sprintf(data, "%d", counter);

    //Clean string output
    memset(string_output, '\0', strlen(string_output));

    //Use another loop to not load the system with more memory to save the users
    for (int j = 0; j < users->last_id; j++) {
        if (strcmp(users->registered_users[j].postal_code, parameters[2]) == 0) {
            sprintf(string_output, "%d %s\n", users->registered_users[j].id, users->registered_users[j].username);
            printF(string_output);

            //TODO allow to send more than 1 frame when it is too large

            sprintf(aux, "*%s*%d", users->registered_users[j].username, users->registered_users[j].id);

            strcat(data, aux);

            //Clean aux string
            memset(aux, '0', 100);


            //Clean string output
            memset(string_output, '\0', strlen(string_output));
        }
    }


    trama = tramaSearchResponse(data);

    // send response with the users found
    write(fd, trama, 256);

    printF("Enviada resposta\n");


}

void data_photo_receive(char *size2, int fd, char *MD5SUM) {
    int file_id = 0;
    char frame_string[256];
    Frame frame;
	User user;

    int size = atoi(size2);
    int number_frame;


	//get user by fd
	user = get_user_by_fd(fd);

    char *trama= malloc(256 * sizeof(char));
    char *name_file = (char *) malloc(sizeof(char) *(strlen(".jpg") + sizeof(user.id)));
    char *pathFoto = (char *) malloc(sizeof(char) * (strlen(serverInfo->directory) + strlen(name_file)));
    char *dades = (char *) malloc(sizeof(char) *(strlen("Guardada com ") + sizeof (user.id) + strlen(".jpg\n")));

    sprintf(name_file, "%d.jpg", user.id);
    sprintf(pathFoto, ".%s/%s", serverInfo->directory, name_file);

    file_id = open(pathFoto, O_CREAT | O_WRONLY | O_TRUNC, 0666);

    if ((size % 240) != 0) {
        number_frame = (size / 240) + 1;
    } else {
        number_frame = (size / 240);
    }

    for (int i = 0; i < number_frame; i++) {

        read(fd, frame_string, 256);
        frame = createFrameFromString(frame_string);

        if (i == (number_frame - 1)) {
            write(file_id, frame.data, size % 240);
        } else {
            write(file_id, frame.data, 240);
        }

    }

    close(file_id);

    sprintf(dades, "Guardada com %d.jpg\n", user.id);
    printF(dades);
    free(dades);

    if (strcmp(MD5Generate(pathFoto),MD5SUM) == 0) {
        trama = sendResponse(1);
    } else {
        trama = sendResponse(2);
    }
    write(fd, trama, 256);


    free(trama);
    free(pathFoto);
    free(name_file);

}


User get_user_by_fd(int fd){
	User aux;

	for(int i = 0; i < num_connections; i++){
		if(open_connections[i].file_descriptor == fd){
			return open_connections[i].user;
		}
	}

	aux.id = -1;
	

	return aux;
}



void read_info_photo_send(Frame frame, int fd) {
    char *parameters[3];
    int i = 0;
    char string_output[100];
	User user;

    //split frame.data to get parameters (MD5SUM)
    char *p = strtok(frame.data, "*");


    while (p != NULL) {
        parameters[i++] = p;
        p = strtok(NULL, "*");
    }
		
	//get user by fd
	user = get_user_by_fd(fd);

    sprintf(string_output, "Rebut send %s de %s %d\n", parameters[0], user.username, user.id);

    printF(string_output);

    memset(string_output, '\0', strlen(string_output));

    data_photo_receive(parameters[1], fd, parameters[2]);
}


void logout(Frame frame) {
    char *parameters[2];
    char string_output[100];
    int i = 0;

    //split frame.data to get parameters
    char *p = strtok(frame.data, "*");


    while (p != NULL) {
        parameters[i++] = p;
        p = strtok(NULL, "*");
    }

    sprintf(string_output, "Rebut logout de %s %s\n", parameters[0], parameters[1]);

    printF(string_output);

    //TODO all stuff necessary to keep the server stable

	//TODO delete connection from open_connections

    free(p);

    printF("Desconnectat d'Atreides\n");

    //Let the thread kill itself
    pthread_detach(pthread_self());
    pthread_cancel(pthread_self());

}


void send_user_photo(int fd, Frame frame) {
    char string_output[100], frame_string[256];
    char *path, *trama, *filename;
    char aux[50];
    int id_user_photo, size, file, num_frames;
	User user;

    strcpy(aux, frame.data);

    filename = (char *) malloc(sizeof(char) * (strlen(aux) + strlen(".jpg")));

    sprintf(filename, "%s.jpg", aux);

    id_user_photo = atoi(frame.data);


    //get user by the fd
	user = get_user_by_fd(fd);

    sprintf(string_output, "Rebut photo %d de %s %d\n", id_user_photo, user.username, user.id);

    printF(string_output);

    path = (char *) malloc(sizeof(char) * (strlen(serverInfo->directory) + strlen(aux)));

    //create path
    sprintf(path, ".%s/%s.jpg", serverInfo->directory, aux);


    //check if the photo exists
    if (access(path, F_OK) == 0) { //photo exists

        //send picture
        size = GetSizeFile(path);

        trama = tramaPhotoPicture(filename, size, MD5Generate(path));

        //TODO check trama correcta (1)

        write(fd, trama, 256);

        memset(string_output, '\0', 100);

        sprintf(string_output, "Enviament %d.jpg\n", id_user_photo);

        printF(string_output);

        //calculate number of frames to send
        num_frames = size / 240;

        if ((size % 240) != 0) { //THe last frame might be shorter
            num_frames++;
        }

        //open file and read picture
        file = open(path, O_RDONLY);

        memset(trama, 0, strlen(trama));

        for (int i = 0; i < num_frames; i++) { //send picture frame by frame

            trama = sendDataPhotoAtreides(file);
            write(fd, trama, 256);

        }

        close(file);

		//wait for Fremen response

		read(fd, frame_string, 256);

		frame = createFrameFromString(frame_string);

		if(frame.type == 'R'){ //ERROR from Fremen with the picture
			printF("La imatge no s'ha rebut correctament!\n");
		}


    } else { //photo does not exists

        printF("No hi ha foto registrada.\n");

        trama = tramaPhotoNotFound();

        //send photo not found frame
        write(fd, trama, 256);

    }

    printF("Enviada resposta\n");


    //TODO free memory used

}


void *run_thread(void *fd_client) {
    int exit = 0;
    int fd = *((int *) fd_client);
    char frame_string[256];
    Frame frame;

    while (!exit) {

        read(fd, frame_string, (sizeof(char) * 256));

        frame = createFrameFromString(frame_string);

        switch (frame.type) {
            case 'C': //LOGIN
                login_user(fd, frame);
                break;
            case 'S': //SEARCH
                search_users(fd, frame);
                break;
            case 'F': //SEND
                read_info_photo_send(frame, fd);
                break;
            case 'P': //PHOTO
                send_user_photo(fd, frame);
                break;
            case 'Q': //LOGOUT
                logout(frame);
                break;
            default: //UNRECOGNIZED
                break;
        }
    }

    close(fd);

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
            servidor.sin_port = htons(serverInfo->port);
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

			num_connections = 0;

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
