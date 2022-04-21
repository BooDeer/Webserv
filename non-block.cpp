// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <map>
#include <iostream>

#define PORT 83

#define CHUNK_SIZE 500
int receive_basic(int s, std::map<int, std::string>& m)
{
    // map(s, hhh)
    // map[s] == ""
    std::cout << "receve data  if == > "   << s << std::endl;
	int size_recv , total_size= 0;
	char chunk[CHUNK_SIZE];
    size_t size_read;
    while ((size_recv =  recv(s , chunk , CHUNK_SIZE , 0)) > 0)// "\r\n\r\n" || endof/chunk
    {
         
        if(m[s].find("\r\n\r\n") != std::string::npos)
        {
                // i = 0;
                std::cout << " find " << std::endl;
                std::cout << "number ==  "  << s << " " << m[s] << std::endl;
                std::cout << "|||||||||||||||||||||||||||||size == > " << size_recv << std::endl;
        }
        else
            m[s].append(chunk);// s == 4   
    }
    
	return total_size;
}

int main(int argc, char const *argv[])
{
    int server_fd, server_fd2, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
 
    char hello[600] = "HTTP/1.1 200 OK\nContent-Type: image/png\nContent-Type: text/html\nContent-Length: 700\n\n<h1>Hello world!<img src=socket.jpg alt=""></h1>";
    
    //int fd = open("non-block.cpp", O_RDONLY);
    // int fd2 = open("non-block.cpp", O_RDONLY);
    // int fd3 = open("non-block.cpp", O_RDONLY);
    // Creating socket file descriptor
    std::string test1;
    std::map<int, std::string> m;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    // if ((server_fd2 = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    // {
    //     perror("In socket");
    //     exit(EXIT_FAILURE);
    // }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    
    address.sin_port =  htons(PORT);
    std::cout << " address.sin_port " <<  address.sin_port << std::endl;
    // memset(address.sin_zero, '\0', sizeof address.sin_zero);
    // fcntl()
   fcntl(server_fd, F_SETFL, O_NONBLOCK);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    fd_set current_sockets, ready_sockets;
    FD_ZERO(&current_sockets);
    FD_SET(server_fd, &current_sockets);
    // FD_SET(server_fd2, &current_sockets);
    // FD_CLR(server_fd2, &current_sockets);
    // FD_CLR(server_fd, &current_sockets);
    while(1)
    {
        i = 1;
        ready_sockets = current_sockets;
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
        {
            // continue;
            perror("select error");
            exit(EXIT_FAILURE);
        }
        int client_socket;
        for (size_t i = 0; i < 9; i++)
        {
            if (FD_ISSET(i, &ready_sockets))
            {
                std::cout << "i ===> " << i << " server  socket ==> " << server_fd <<  std::endl;
                if(i == server_fd)
                {
                    std::cout << "check is server fd read " << std::endl;
                    client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
                    std::cout << "client socket ===> " << client_socket << std::endl; 
                    FD_SET(client_socket, &current_sockets);
                    
                //    close(client_socket);
                }
                else
                {
                    // handle request  
                    printf("------------------Hello message sent-------------------\n");
                    receive_basic(client_socket, m);
                    std::cout << "----------------------------------------------------client >> " << i << std::endl;
                    std::cout << "test 2 ==> " << m[i] << std::endl;
                    write(client_socket , hello , strlen(hello));
                    //  int fd = open("socket.jpg", O_RDONLY);
                    //  char *save = new char[5000];
                    //  read(fd, save, 5000);
                    // if (i == 5)
                    // {
                    //     std::cout << "hna 2 " << std::endl;
                    //     // char test[5500] = "HTTP/1.1 200 OK\nContent-Type: image/png\nContent-Type: text/html\nContent-Length: 10000\n\n";
                    
                    //     //  write(client_socket , test , strlen(test));
                    //       write(client_socket , save , strlen(save));
                    //     // write()
                    // }
                    // close(client_socket);
                    // FD_CLR(i, &current_sockets);
                }
            }
            

        }

    }
    return 0;
}







// int main()
// {
//     fcntl(0, F_SETFL, O_NONBLOCK);
//     char *
//      printf("%d\n", read(0, NULL, 10));
//      return 0;
// }