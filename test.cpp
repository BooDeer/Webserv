#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <iostream>

#define PORT 9091

int		client_socket;
void	close_server(int i)
{
	printf("Reached here\n");
	close(client_socket);
}
int receive_basic(int s)
{
	int size_recv = 0 , total_size= 0;
	char *chunk = malloc(500);
	int i = 0;
    int count = 0;
	//loop
    int out = open("data222.txt", O_WRONLY | O_APPEND | O_TRUNC);
	while(i<1)
	{
    signal(SIGINT, close_server);

		memset(chunk ,0 , 500);	//clear the variable
       recv(s , chunk , 500, 0);
        printf("===>%d\n", size_recv);
		if((size_recv ) < 0)
		{
            printf("-----> %d\n", size_recv);
			return (0);
		}
		else
		{
            printf("-----> %d\n", size_recv);
			total_size += size_recv;
			
            write(out, chunk, 500);
		}
        i++;
	}
	
	return total_size;
}
int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char *hello = "HTTP/1.1 200 OK\nContent-Type: image/png\nContent-Type: text/html\nContent-Length: 700\n\n<h1>Hello world!</h1><img src='/Users/fbouibao/Desktop/webserv/icons8-home-32.png' alt=''>";
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    // fcntl()
    fcntl(server_fd, F_SETFL, O_NONBLOCK);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    fd_set current_sockets, ready_sockets;
    FD_ZERO(&current_sockets);
    FD_SET(server_fd, &current_sockets);
    // FD_SET(server_fd, &current_sockets);
	// i = 1;
	// i = i << 1;
	// i = i << 1;
	// i = i << 1;
	// std::cout <<i << std::endl;
    while(1)
    {
        ready_sockets = current_sockets;
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
        {
            perror("select error");
            exit(EXIT_FAILURE);
        }
        // int client_socket;
        for (size_t i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &ready_sockets))
            {
                if(i == server_fd)
                {
                    client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
                    
                    FD_SET(client_socket, &current_sockets);
                    receive_basic(client_socket);

                    write(client_socket , hello , strlen(hello));
                    printf("------------------Hello message sent-------------------\n");
                    close(client_socket);
                }
            }
            else
            {
                FD_CLR(i, &current_sockets);
            }

        }
        
        
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            close(new_socket);
            //exit(EXIT_FAILURE);
        }
        
        char *buffer = malloc(sizeof(char) * 1000);
        char *buffer2 = malloc(sizeof(char) * 1000);
        int fda = open("data", O_CREAT | O_RDWR | O_TRUNC | O_APPEND, 777);
        // valread = read( server_fd , buffer2, 1000);
        // buffer2[999] = '\0';
        // write(fda , buffer2 , strlen(buffer2));
        buffer[999] = '\0';

        int bytes = recv(new_socket, buffer, 1000, 0);
        receive_basic(client_socket);

       
       

    }
    return 0;
}



