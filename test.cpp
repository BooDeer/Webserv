#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>


#define PORT 4118

#define MaxBufferLength 1024 // set the size of data you want to recieve from Server


int main()
{
    int sockFd, bytesRead= 1, bytesSent;

    char buffer[MaxBufferLength];

    struct sockaddr_in server, client;


    server.sin_port= htons(PORT);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    sockFd = socket(AF_INET, SOCK_STREAM, 0);


    if(sockFd < 0)
        printf("Unable to open socket\n");

    int connectionSocket = connect(sockFd, (struct sockaddr *) &server, sizeof(struct sockaddr) );

    if(connectionSocket < 0)
        perror("connection not established\n");


    int fd = open("helloworlds.txt",O_CREAT | O_WRONLY,S_IRUSR | S_IWUSR);  

    if(fd == -1)
        perror("couldn't openf iel");

    while(bytesRead > 0)
    {           

        bytesRead = recv(sockFd, buffer, MaxBufferLength, 0);

        if(bytesRead == 0)
        {

            break;
        }

        printf("bytes read %d\n", bytesRead);

        printf("receivnig data\n");

        bytesSent = write(fd, buffer, bytesRead);


        printf("bytes written %d\n", bytesSent);

        if(bytesSent < 0)
            perror("Failed to send a message");

    }


    close(fd);

    close(sockFd);

    return 0;

}