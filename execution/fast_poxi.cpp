#include "execution.hpp"
#include "../webserv.hpp"

#define CHUNK_SIZE 10000



void prepare_socket(std::map<unsigned short, std::string>::iterator& it, int &save) // create  the socket.
{
    struct sockaddr_in server_info;
    // std::map<unsigned short, std::string>::iterator it = server.begin();

    std::cout << "------------crearte socket----------- with port "  <<  (*it).first << std::endl;
    save = socket(PF_INET, SOCK_STREAM, 0);
    if (save < 0)
    {
        std::cout <<  "socket error" << std::endl;
        exit(1);
    }
    server_info.sin_family = PF_INET; // verify is ip4 
    server_info.sin_addr.s_addr = inet_addr((*it).second.c_str()); // convert ip(127.0.0.1) to in_addr_t  
    server_info.sin_port =  htons((*it).first);
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

void receive_basic(int s, fd_set &current_sockets, int fd_socket,  std::map<int , data> &req)// s ==> client socket
{

    char chunk[CHUNK_SIZE];
    memset(chunk ,0 , CHUNK_SIZE);
    int size_read = recv(s , chunk , CHUNK_SIZE, 0);
    if(size_read < 0 )
    {
        std::cout << "error in recv :(" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "size read = " << size_read << "byte" << std::endl; 
    req[s].size_read_complet = req[s].lenth  - size_read;
    // parse here
    // CHECK IF POST
    std::stringstream check(chunk);
    std::string tmp;
    std::cout << "socket number " << s << std::endl;
    if(req[s].is_header ==  false) // parsing the header.
    {
        std::getline(check, tmp); // get first line in header
        first_line(tmp, req[s]); // just parse first line
        parsing_header(check, req[s]); // paring all headers
        std::cout << "name of file get here ->  " <<  req[s]._fileName  << "  check limit == >  " << req[s].lenth  << " check server block  " << req[s].config_block.__ClientLimit << std::endl;
        if(req[s].config_block.__ClientLimit != 0) // if __ClientLimit equel 0 do nothing
        {
            if(req[s].lenth > req[s].config_block.__ClientLimit)
                throw 413;
        }
        tmp = std::string(chunk);
        tmp.erase(0, tmp.find("\r\n\r\n") + 4);
        std::cout << "body ====> " << "|" << tmp  << "|" << std::endl;
        req[s].is_header = true;
        // here
        std::cout << "end of parsing " << std::endl;
    }
    // else write if POST on file or pipe
     if(req[s].is_header ==  true && tmp.length() != 0 && req[s].method == "POST") // add delete if need
     {
         // check limmite size and throw error
        req[s].create_file(fd_socket, s);
        write(req[s]._fileFd, tmp.c_str(),  tmp.length());
        tmp.clear();
     }
     // req[s].is_header == false 
     else if (req[s].is_header == true && req[s].method == "POST") // forget why :(
     {
        std::cout << "chunk here -------> " << req[s]._fileName << std::endl;
	    write(req[s]._fileFd, chunk, std::strlen(chunk));
     }
    // if recv return 0  close connectoin  /// check limite size and close client socket
    if(size_read == 0)
    {
        // close(s);
       // req[s].remove = true; // set remove to remove data from map
       // FD_CLR(s, &current_sockets);
        std::cout << "close and clear file" << std::endl;
    }
	usleep(10);
	std::cout << " ------------------------- chuck --------------------" << std::endl;
    memset(chunk , 0 , CHUNK_SIZE);	//clear the variable
}

void start_server(int *fd_savior, fd_set *socket_list, size_t servers, ConfigFile &conf)
{
    struct timeval tm;
   
    tm.tv_sec = 0;
    tm.tv_usec = 10;
    std::cout << socket_list[0].fds_bits[0] << std::endl;
    fd_set read_check;
    fd_set  write_check;

    int client_socket;
    char hello[82] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 20\n\n<h1>Hello world!</h1>";
    std::map<int , data> request_info; // std::map<client_socket, data struct(contains headers ect)>
    while(1)
    {
        for(int i = 0; i <  servers; i++)
        {
            read_check = socket_list[i];
            write_check =  read_check;
            if(select(FD_SETSIZE, &read_check, &write_check, NULL, &tm) < 0) // add ready to readls 
            {
                std::cout << " select problem " << std::endl;
                exit(EXIT_FAILURE);
            }
            for(size_t j = 0 ; j < FD_SETSIZE; j++)
            {
                if(FD_ISSET(j, &read_check))
                {
                    std::cout << " j is set == > " << j << std::endl;
                    data tmp;
                    if (j == fd_savior[i])
                    {
                        std::cout << "fd from socket " << std::endl;
                        client_socket = accept(fd_savior[i], NULL, NULL); // accept connection from browser
                        if (client_socket < 0 )
                        {
                            std::cout << " accept  error " << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        //tmp.create_file(fd_savior[i],  client_socket); // create file of request
						//std::cout << "Pushing data struct in the map :D with fd of : "<< tmp._fileFd << std::endl;
                        request_info[client_socket] = tmp; // add to map
                        FD_SET(client_socket, &socket_list[i]); // set client socket(return of accept) to set
                        // FD_SET(client_socket, &read_check); 
                        std::cout << "-------------> end of scope fd socket " << std::endl;
                        usleep(10);
                    }
                    else
                    {
                            // if not socket fd start to read request and parse
                        std::cout << " ----------------------------------- in else in read request -------------------------" << std::endl;
                        request_info[j].config_block = conf.__Servers[i];
                        receive_basic(j, socket_list[i], fd_savior[i], request_info);
                    }
                }
                if(FD_ISSET(j, &write_check))
                {
                        // scope for check if you can write in client fd
                        // std::cout << request_info[j].size_read_complet << std::endl;
                    if(request_info[j].size_read_complet < 0) // if we complet Content-Length send responce 
                    {
                        write(j, hello, strlen(hello)); // send
                        // FD_CLR(j, &read_check); 
                        FD_CLR(j, &socket_list[i]);
                        close(j);
                        // remove(request_info[j]._fileName.c_str())
                        request_info.erase(j); // erase socket data parsing from map after send response
                    }
                }
            }
          

        }
    }
}

void filterByServer(ConfigFile &conf, std::map<unsigned short, std::string> &unq)
{
    /* 
    
        error to check rn: no server_names, same server_names, different server_names(should work), unique host:port after filtring.
     */
    for(int i = 0; i < conf.__Servers.size(); i++)
        unq[conf.__Servers[i].__Port] = conf.__Servers[i].__Host;
}

void install_servers(ConfigFile &conf) // intall servers 
{
    // create socket first;
    //add parameters from config class
    // change servers
    // here
    std::map<unsigned short, std::string> uniqueServers; // host:port

    filterByServer(conf, uniqueServers); // This is wrong, it takes unique port (it discards the case of similar ip and different host servers)
    size_t server_size = uniqueServers.size();
     std::map<unsigned short, std::string>::iterator it = uniqueServers.begin();
    for(; it != uniqueServers.end() ; it++)
        std::cout <<  "port == > "  << it->first << std::endl;

    // while (1)
    //     ;
    int fd_savior[server_size]; // save fd socker server 
    fd_set socket_list[server_size]; // create fd_set for each server
    std::map<unsigned short, std::string>::iterator it2 =  uniqueServers.begin();
    for(int i  = 0; i < server_size; i++)
    {
        prepare_socket(it2, fd_savior[i]); // create one socket each server in config file
        FD_ZERO(&socket_list[i]); // clear bit fd_set to 0
        FD_SET(fd_savior[i], &socket_list[i]); // set bit in ft_set bytes 
        it2++;
    }
   start_server(fd_savior, socket_list, server_size, conf); // all work from select to send response
}


/* 

    location /kapouet {
        <config>
    }
    location / {
        <config>
    }
    }
 */