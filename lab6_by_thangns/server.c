#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>



char client_message[2000];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int numberThread = 1;

void print_dir(int fd, const char *dirname) {
    errno = 0;
    DIR *dir = opendir(dirname);

    if (errno != 0) {
        char * buf = (char *)malloc(2048);
        sprintf(buf, "Can not open \"%s\". Error %s\n\n", dirname, strerror(errno));
        write(fd, buf, strlen(buf));
        free(buf);
        errno = 0;
        return;
    }

    char * buf = (char *)malloc(2048);
    sprintf(buf, "DIR \"%s\":\n", dirname);
    write(fd, buf, strlen(buf));
    free(buf);

    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL){
        char * buf = (char *)malloc(2048);
        sprintf(buf, "%s\n", ent->d_name);
        write(fd, buf, strlen(buf));
        free(buf);
    }

    char nn[] = "\n\n";
    write(fd, nn, strlen(nn));
    closedir(dir);
}

void read_client(int client) {
    int len = 0;
    char *buf = (char*)malloc(2048 * sizeof(char));
    int bytes_read;
    char *request = NULL;
    while ((bytes_read = read(client, buf, 2048)) > 0) {
        request = realloc(request, len + bytes_read);
        memcpy(request + len, buf, bytes_read);
        len += bytes_read;
        printf("request: \"%s\"\n", request);
        break;
    }

    char *dirs = strtok(request, "\n");
    while (dirs != NULL) {
        print_dir(client, dirs);
        dirs = strtok(NULL, "\n");
    }
    write(client, "\0", 1);
    free (request);
    close(client);
}

void * socketThread(void *arg)
{
    printf("Create Thread %d \n", numberThread);
    int newSocket = *((int *)arg);
   // recv(newSocket , client_message , 2000 , 0);
    // Send message to the client socket
    pthread_mutex_lock(&lock);
    read_client(newSocket);

    pthread_mutex_unlock(&lock);
    sleep(2);
    printf("Exit socketThread %d \n", numberThread);
    numberThread++;
    close(newSocket);
    pthread_exit(NULL);
}



int main(int argc,char **argv){
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    int port, opt = 1;
    char *ip = "";
    
    //get option
    while ((opt = getopt(argc,argv,"a:p:"))!=-1){
        switch(opt){
          case 'a':
            ip = optarg;
            break;
          case 'p':
            port = atoi(optarg);
            break;
          default:
            printf("wrong option\n");
            exit(1);
        }
    }
    

    
    //init server
    if ((serverSocket = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return 0;
    }
    
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR,&opt,sizeof(opt)) == -1){
        perror("setsockopt");
        return 0;
    }
    
    in_addr_t INADDR_LOCAL = inet_addr("172.0.0.1");

    // Configure settings of the server address struct
    // Address family = Internet
    serverAddr.sin_family = AF_INET;
    //Set port number, using htons function to use proper byte order
    serverAddr.sin_port = htons(port);
    //Set IP address to localhost
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    
    if (ip == "") serverAddr.sin_addr.s_addr = INADDR_ANY; else serverAddr.sin_addr.s_addr = inet_addr(ip);

    //Set all bits of the padding field to 0
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    //Bind the address struct to the socket
  //  bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr))<0){
        perror("bind");
        return 0;
    }
    
    //Listen on the socket, with 40 max connection requests queued
    if(listen(serverSocket,5)==0)
        printf("Listening\n");
    else
        printf("Error\n");
    pthread_t tid[60];
    int i = 0;
    printf("%s listening to port %d\n",inet_ntoa(serverAddr.sin_addr),port);

    while(1)
    {
        //Accept call creates a new socket for the incoming connection
        addr_size = sizeof serverStorage;
        newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);

        //for each client request creates a thread and assign the client request to it to process
        //so the main thread can entertain next request
        if( pthread_create(&tid[i], NULL, socketThread, &newSocket) != 0 )
            printf("Failed to create thread\n");
        if( i >= 50)
        {
            i = 0;
            while(i < 50)
            {
                pthread_join(tid[i++],NULL);
            }
            i = 0;
        }
    }
    return 0;
}
