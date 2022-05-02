#include <cstdint>
#include <cstring>
#include <ostream>
#include <sys/socket.h> // sokcet header :p
#include <iostream>
#include <stdlib.h>
#include <netinet/in.h> // structer sockaddr_in header
#include <arpa/inet.h> // header for inet_addr


void prepare_socket(char *ip, short port, int &save)
{
    struct sockaddr_in server_info;
    std::cout << "------------crearte socket-----------" << std::endl;
    save = socket(AF_INET, SOCK_STREAM, 0);
    if (save < 0)
    {
        std::cout <<  "socket error" << std::endl;
        exit(1);
    }
    server_info.sin_family = AF_INET; // verify is ip4 
    server_info.sin_addr.s_addr = inet_addr(ip);
    server_info.sin_port =  htons(port);

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


void install_servers()
{
    
}


void start_server(char *ip, short port)
{
    // all select work 
    
     

}



int main()
{
    // check error args
    
    int sockets_fd[65535]; //save  all fd 
    memset(sockets_fd, -1, 65535);
    char ip[10] = "127.0.0.1";
    start_server(ip, 9999);
    
    
}


