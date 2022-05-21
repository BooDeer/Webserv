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

int receive_basic(int s, fd_set &current_sockets, int fd_socket,  std::map<int , data> &req)
{

    int size_recv , total_size= 0;
    char chunk[CHUNK_SIZE];
    memset(chunk ,0 , CHUNK_SIZE);
    int size_read;
    size_read = recv(s , chunk , CHUNK_SIZE, 0);
    // parse here
    // CHECK IF POST
    std::stringstream check(chunk);
    std::string tmp;
    std::cout << "socket number " << s << std::endl;
    if(req[s].is_header ==  false) // parsing the header.
    {
        // std::string tmp;
        // parse here requset check errors and throw or return
        // get line 
        std::getline(check, tmp); // get first line in header
        // if (tmp.find("HTTP") != std::string::npos)
        first_line(tmp, req[s]);
        // try catch
        // check_first_line(req, ) // -------------- check throw
        // if (tmp.find("\r\n\r\n") != std::string::npos)
        parsing_header(check, req[s]);
        tmp = std::string(chunk);
        
        tmp.erase(0, tmp.find("\r\n\r\n") + 4);
        std::cout << "|" << tmp  << "|" << std::endl;
        req[s].is_header = true;
        // here
       
        std::cout << "end of parsing " << std::endl;
    }
    // else write if POST on file or pipe
    
     if(req[s].is_header ==  true && tmp.length() != 0)
     {	        
         std::cout << "name of file get here -> " << req[s]._fileName << std::endl;
         write(req[s]._fileFd, tmp.c_str(),  tmp.length());
         tmp.clear();
     }
     else if (req[s].is_header == false)
     {
        std::cout << "chunk here -------> " << req[s]._fileName << std::endl;
	    write(req[s]._fileFd, chunk, std::strlen(chunk));
     }
    // if recv return 0  close connectoin  /// check limite size and close client socket
    if(size_read == 0)
    {
        close(s);
        req[s].remove = true; // set remove to remove data from map
        FD_CLR(s, &current_sockets);
        std::cout << "close and clear file" << std::endl;
    }
	usleep(10);
	std::cout << " ------------------------- chuck --------------------" << std::endl;
    memset(chunk , 0 , CHUNK_SIZE);	//clear the variable
    return total_size;
}

// // template<class T>
// // void create_sockets(T data, int *&fds)
// // {
// //     for(int i = 0; i <= data.nb_of_servers; i++)
// //     {
// //         prepare_socket(data.ip, data.port, fds[i]);
// //     }
// //     //return fds;
// // }

// // template<class T>




void start_server(int *fd_savior, fd_set *socket_list, size_t servers)
{
    struct timeval tm;
   
    tm.tv_sec = 0;
    tm.tv_usec = 10;
    std::cout << socket_list[0].fds_bits[0] << std::endl;
    fd_set read_check[servers];
    int client_socket;
    char hello[82] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 20\n\n<h1>Hello world!</h1>";
    std::map<int , data> request_info; // std::map<client_socket, data struct(contains headers ect)>
    // test map 
    // test int count 
    int save1;
    // std::vector<data> test11;
    while(1)
    {
        for(int i = 0; i <  servers; i++)
        {
            read_check[i] = socket_list[i];
            if(select(FD_SETSIZE, &read_check[i], NULL, NULL, &tm) < 0) // add ready to readls 
            {
                std::cout << " select problem " << std::endl;
                exit(EXIT_FAILURE);
            }
            for(size_t j = 0 ; j < FD_SETSIZE; j++)
            {
                if(FD_ISSET(j, &read_check[i]))
                {
                    data tmp;
                    if (j == fd_savior[i])
                    {
                        client_socket = accept(fd_savior[i], NULL, NULL); // accept connection from browser
                        tmp.create_file(fd_savior[i],  client_socket); // create file of request
						std::cout << "Pushing data struct in the map :D with fd of : "<< tmp._fileFd << std::endl;
                        request_info[client_socket] = tmp; // add to map
                        FD_SET(client_socket, &socket_list[i]); // set client socket(return of accept) to set 
                        // usleep(10);
                    }
                    else
                    {
                        std::cout << "REACHED HERE AFTER CREATING THE FILE: " << request_info[j]._fileName << "with fd of : "<< request_info[j]._fileFd<< std::endl;
						// std::cout << "here " << std::endl;
                        receive_basic(j, socket_list[i], fd_savior[i], request_info);
                        write(j, hello, strlen(hello)); // send
                        // remove(request_info[j]._fileName.c_str())
                        request_info.erase(j); // erase socket data parsing from map after send response
                        
						// FD_CLR(fd_savior[i], &socket_list[i]);
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
   start_server(fd_savior, socket_list, server_size); // all work from select to send response
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