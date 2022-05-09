#include <cstdint>
#include <ostream>
#include <sys/socket.h> // sokcet header :p
#include <sys/select.h> //  select header
#include <iostream>
#include <stdlib.h>
#include <unistd.h> // for close 
#include <fstream>  // header for create file 
#include <netinet/in.h> // structer sockaddr_in header
#include <arpa/inet.h> // header for inet_addr
#include <fcntl.h> // fcntl hreader 
#include <stdio.h> // for remove() function

#define servers 100
#define CHUNK_SIZE 10000
void prepare_socket(char *ip, short port, int &save)
{
    struct sockaddr_in server_info;
    std::cout << "------------crearte socket----------- with port " << port << std::endl;
    save = socket(PF_INET, SOCK_STREAM, 0);
    if (save < 0)
    {
        std::cout <<  "socket error" << std::endl;
        exit(1);
    }
    server_info.sin_family = PF_INET; // verify is ip4 
    server_info.sin_addr.s_addr = inet_addr(ip); // convert ip(127.0.0.1) to in_addr_t  
    server_info.sin_port =  htons(port);
    fcntl(save, F_SETFL, O_NONBLOCK);
    if(bind(save, (sockaddr *)&server_info, sizeof(server_info)) < 0)
    {
        std::cout << "bind error" << std::endl;
        exit(1);
    }
    if(listen(save, INT32_MAX) < 0)
    {
        std::cout << "listen error" << std::endl;
        exit(1);
    }
}

int receive_basic(int s, fd_set &current_sockets)
{

//   std::cout << " test hna 200 " << std::endl;
  int size_recv , total_size= 0;
  char chunk[CHUNK_SIZE];
  memset(chunk ,0 , CHUNK_SIZE);

        int size_read;
     std::fstream fs;
      
//      std::string name = "test_file";
    //   name  + std::to_string(s);
        fs.open ("just_test", std::fstream::in | std::fstream::out | std::fstream::binary);
        size_read = recv(s , chunk , CHUNK_SIZE, 0);
        if(size_read == 0)
        {
            close(s);
            std::cout << "close and clear file" << std::endl;
            // open again // remove
            FD_CLR(s, &current_sockets);
        }
            std::cout << chunk << std::endl;
            std::string bood(chunk);
            if (bood.find("\r\n\r\n") != std::string::npos)
                std::cout << "test is working" << std::endl;
            fs << chunk;
          std::cout << " ------------------------- chuck --------------------" << std::endl;
      memset(chunk , 0 , CHUNK_SIZE);	//clear the variable
    return total_size;
}

// template<class T>
// void create_sockets(T data, int *&fds)
// {
//     for(int i = 0; i <= data.nb_of_servers; i++)
//     {
//         prepare_socket(data.ip, data.port, fds[i]);
//     }
//     //return fds;
// }

// template<class T>




void start_server(int *fd_savior, fd_set *socket_list)
{
    struct timeval tm;
    for(int i = 0; i < 4; i++)
    {
        std::cout << fd_savior[i] << std::endl;
    }
    tm.tv_sec = 0;
    tm.tv_usec = 10;
    // struct sockaddr_in address;
    // socklen_t addrlen = sizeof(ad/dress);
    fd_set read_check[servers];
    int client_socket;
    char hello[82] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 20\n\n<h1>Hello world!</h1>";
    // int selewct_check;
    while(1)
    {
        for(int i = 0; i <  servers; i++)
        {
            read_check[i] = socket_list[i];
            if(select(FD_SETSIZE, &read_check[i], NULL, NULL, &tm) < 0)
            {
                std::cout << " select problem " << std::endl;
                exit(EXIT_FAILURE);
            }
            for(size_t j = 0 ; j < FD_SETSIZE; j++)
            {
                if(FD_ISSET(j, &read_check[i]))
                {
                    // std::cout << " here " << std::endl;
                    if (j == fd_savior[i])
                    {
                        client_socket = accept(fd_savior[i], NULL, NULL);
                        FD_SET(client_socket, &socket_list[i]);
                        // std::cout << " is out " << std::endl;
                        usleep(10);
                    }
                    else
                    {
                        receive_basic(client_socket, socket_list[i]);
                        // std::cout << "send response " << std::endl;
                        // send(sj, hello, )
                        write(j, hello, strlen(hello));
                    }
                }
            }
        }
    }
}

void install_servers()
{
    // create socket first;
    int fd_savior[servers];
    char ip[]= "127.0.0.1";
    short port = 2008;
    fd_set socket_list[servers];
   for(int i  = 0; i < servers; i++)
   {
        prepare_socket(ip, port + i, fd_savior[i]);
        FD_ZERO(&socket_list[i]);
        FD_SET(fd_savior[i], &socket_list[i]);
   }
    start_server(fd_savior, socket_list);
}

// test 1 
int main()
{
    install_servers();
}