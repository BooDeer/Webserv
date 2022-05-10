#include <stdio.h>
<<<<<<< HEAD
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
=======
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <map>
#include <iostream>

#define PORT 70

#define CHUNK_SIZE 1000


int receive_basic(int s, std::map<int, std::string>& m, fd_set &current_sockets)
{
  // map(s, hhh)
  // map[s] == ""
  std::cout << " test hna 200 " << std::endl;
  int size_recv , total_size= 0;
  char chunk[CHUNK_SIZE];
  memset(chunk ,0 , CHUNK_SIZE);
  // int fd = open("helloworlds.txt",O_CREAT | O_WRONLY |  O_APPEND);

  int size_read;
//   while ((size_read =recv(s , chunk , CHUNK_SIZE, 0)) > 0)// "\r\n\r\n" || endof/chunk
//     {
        size_read =recv(s , chunk , CHUNK_SIZE, 0);
      m[s].append(chunk);
      std::cout << "app ==> " << m[s] << std::endl;
        if(m[s].find("\r\n\r\n") != std::string::npos)
        {
          // i = 0;
          std::cout << " ------------------------- bruh --------------------" << std::endl;
          FD_CLR(s, &current_sockets);
          return 666;
          // std::cout << "number ==  "  << s << " " << m[s] << std::endl;
          // std::cout << "|||||||||||||||||||||||||||||size == > " << size_recv << std::endl;
        }
      // else
      // {
      memset(chunk ,0 , CHUNK_SIZE);	//clear the variable

      //      std::cout << "app ==> " << m[s] << std::endl;
      //     //  std::cout << "chuck " << chunk << std::endl;
      // }
    // }

  return total_size;
}
>>>>>>> main


int main()
{
<<<<<<< HEAD
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
=======
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
  int faical = 0;
  // if ((server_fd2 = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  // {
  //     perror("In socket");
  //     exit(EXIT_FAILURE);
  // }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;

  address.sin_port =  htons(PORT);
  std::cout << " address.sin_port " <<  htonl(PORT) << std::endl;
  // memset(address.sin_zero, '\0', sizeof address.sin_zero);
  // fcntl()
  int save_data = 0;
  fcntl(server_fd, F_SETFL, O_NONBLOCK);
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
      perror("In bind");
      exit(EXIT_FAILURE);
    }
  if (listen(server_fd, 50) < 0)
    {
      perror("In listen");
      exit(EXIT_FAILURE);
    }
  int i = 0;
  fd_set current_sockets, ready_sockets, write_fd;       
  FD_ZERO(&current_sockets);
  
  FD_SET(server_fd, &current_sockets);
  int d = 0;
                            char *wldfaicla;
                          wldfaicla = (char *)malloc(50000 * sizeof(char));


  while(1)
    {
      ready_sockets = current_sockets;
        write_fd =  ready_sockets;
      printf("\n+++++++ Waiting for new connection ++++++++\n\n");
      if (select(FD_SETSIZE, &ready_sockets, &write_fd, NULL, NULL) < 0)
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
              //std::cout << "i ===> " << i << " server  socket ==> " << server_fd <<  std::endl;
              if(i == server_fd)
                {
                  //std::cout << "check is server fd read " << std::endl;
                  client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
                  //  std::cout << "client socket ===> " << client_socket << std::endl;
                  FD_SET(client_socket, &current_sockets);

                  //    close(client_socket);
                }
              else
                {
                  // handle request
                  printf("------------------Hello message sent-------------------\n");
                  int j = receive_basic(client_socket, m, current_sockets);
                  //std::cout << "----------------------------------------------------client >> " << i << std::endl;

                  //std::cout << "test 2 ==> " << m[i] << std::endl;
                  // std::cerr << "Debugging: client_socket: " << client_socket << std::endl << "buffer: " << hello << std::endl;
                  // send(client_socket, hello, strlen(hello), 0);
                  if (j == 666)
                    {
                      faical++;
                      std::cout << " faical 3ami9" << std::endl;
                      if(faical  == 1)
                        {
                          write(client_socket , hello , strlen(hello));
                            // close(client_socket);
                            save_data = client_socket;
                        }
                      else if (faical == 2)
                        {
                            std::cout << " mara wela " << std::endl;
                          char koko[29000];
                          char hello1[50600] = "HTTP/1.1 200 OK\nContent-Type: image/jpg\nContent-Length: 21560\r\n\r\n"; // add size image 
                          memset(koko, 0, 29000);
                          int fd2 = open("/Users/ssamadi/Desktop/Webserv/socket.jpg", O_RDONLY);
                          int size = read(fd2, koko, 29000);
                          std::cout << "size is === > " << size  << std::endl;
                          //char *wldfaicla = strjoin1(hello1, koko);
                          //char *wldfaicla = (char *)memcpy(hello1 + strlen(hello1), koko, size);
                        //   char *wldfaicla;
                        //   wldfaicla = (char *)malloc(50000 * sizeof(char));
                          memcpy(wldfaicla, hello1, strlen(hello1));
                          memcpy(wldfaicla + strlen(hello1), koko, size);
                          write(client_socket, wldfaicla, strlen(hello1) + size);
                        // send()
                         // FD_CLR(i, &current_sockets);
                         std::cout << "anaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << std::endl;
                        close(client_socket);
                        close(save_data);
                        }
                      //close(client_socket);
                    }
                  // std::cout << "Current value: " << d << std::endl;
                }
            }

        }

    }
  return 0;
  }
>>>>>>> main
