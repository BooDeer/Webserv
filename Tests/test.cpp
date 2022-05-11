#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <map>
#include <iostream>
#include <fstream> 
#include <arpa/inet.h>


int main()
  {
    struct sockaddr_in server_info;
    int save;
    save = socket(PF_INET, SOCK_STREAM, 0);
    if (save < 0)
    {
        std::cout <<  "socket error" << std::endl;
        exit(1);
    }
    server_info.sin_family = PF_INET; // verify is ip4 
    server_info.sin_addr.s_addr = inet_addr("127.0.0.1"); // convert ip(127.0.0.1) to in_addr_t  
    server_info.sin_port =  htons(2000);
    // fcntl(save, F_SETFL, O_NONBLOCK);
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
    fcntl(save, F_SETFL, O_NONBLOCK);
    int a[32];
    // fd_set ready;
    fd_set ready;
    FD_ZERO(&ready);
    for(int  i = 0; i < 32; i++)
    {
      FD_SET(i, &ready);
    }
    int i = 4294967295;
    // std::cout << " is ==> " << ready.fds_bits[0] << std::endl; // 32 * 128 / 4
    std::cout << sizeof(fd_set) << std::endl;
    // select(5, &ready, NULL, NULL, NULL);
    // for(int i = 0; i < 5 ; i++)
    // {
    //   accept(save, NULL, NULL);

    // }
  }