#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>
#include <unistd.h>


int main(int argc,char** argv){
    printf("In thread\n");
    char message[1000];
    char buffer[1024];
    int clientSocket;
    struct sockaddr_in serverAddr;
    int port=-1,opt;
    char* ip = "127.0.0.1";
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
            return 0;
        }
    }
    
    
    socklen_t addr_size;
    
    // remind set PORT
    if (port == -1) {
        printf("[!!!]PORT is not set\n");
        exit(1);
    }
    
    //init Client
    if ((clientSocket = socket(PF_INET,SOCK_STREAM,0))==-1){
        perror("socket");
        return 1;
    }

    if (setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR,&opt,sizeof(opt)) == -1){
        perror("setsockopt");
        return 1;
    }
    
    //Configure settings of the server address
    // Address family is Internet
    serverAddr.sin_family = AF_INET;
    //Set port number, using htons function

    serverAddr.sin_port = htons(port);
    //Set IP address to localhost
    if (ip == "") serverAddr.sin_addr.s_addr = INADDR_ANY; else     serverAddr.sin_addr.s_addr = inet_addr(ip);

    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    //Connect the socket to the server using the address
    addr_size = sizeof serverAddr;
    
    if (connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size) <0){
        perror("Connect");
        exit(1);
    }
    
    
    //send
    char *nameOfDir = argv[optind];
    write(clientSocket, nameOfDir, strlen(nameOfDir));

    //Read the message from the server into the buffer
    char *buf = (char*)malloc(2048 * sizeof(char));
    int len = 0;
    int bytes_read;
    char *response = NULL;
    while ((bytes_read = read(clientSocket, buf, 2048)) > 0) {
        response = realloc(response, len + bytes_read);
        memcpy(response + len, buf, bytes_read);
        len += bytes_read;
        if (*(response + len - 1) == '\0'){
            break;
        }
    }
    printf("Response: %s\n", response);


    close(clientSocket);
    pthread_exit(NULL);
    return 0;
}
