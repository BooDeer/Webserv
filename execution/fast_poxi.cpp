#include "execution.hpp"
#include "../webserv.hpp"

#define CHUNK_SIZE 65536

/**
 * Creates the server socket, bind it, sets it to non-blocking and starts listening.
 * 
 * @param {it:string} Host (e.g: 127.0.0.1).
 * @param {save:number} Server's socket.
 * @return {void}.
 */
void prepare_socket(std::pair<std::string, unsigned short> pair, int &save)
{
    
    struct sockaddr_in server_info;

    std::cout << "------------crearte socket----------- with port "  <<  pair.second << std::endl;
    if ((save = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		exitMessage(1, "socket error");
    server_info.sin_family = PF_INET;
    server_info.sin_addr.s_addr = inet_addr(pair.first.c_str());
    server_info.sin_port =  htons(pair.second);
    fcntl(save, F_SETFL, O_NONBLOCK);
    if(bind(save, (sockaddr *)&server_info, sizeof(server_info)) < 0)
		exitMessage(1, "bind error");
    if(listen(save, INT32_MAX) < 0)
		exitMessage(1, "listen error");
}
void findServerBlock(data& req, ConfigFile& conf) // serverBlock, data struct, host(servernames/host), port
{
    std::vector<ServerBlock>::iterator it;

    it = conf.__Servers.begin();

    for (; it != conf.__Servers.end(); it++)
    {
        std::cout << "ip or server name is ===> " << req.ip_server_name << std::endl;
        if("0.0.0.0" == req.ip_server_name)
            req.ip_server_name = "127.0.0.1";
        if ((*it).__Port == req.port && ( (*it).__Host == req.ip_server_name || (*it).__ServerNames[0] == req.ip_server_name))
        {
            // std::cout << "======================================================================================" << std::endl;
            // std::cout << "port from server block: " << (*it).__Port  << std::endl;
            // std::cout << "host from server block: " << (*it).__Host << std::endl;
            // std::cout << "======================================================================================" << std::endl;
            req.config_block = *it;
            // req.location = (*it).__Locations;
            // std::cout << "route of the above: " << req.config_block.__Locations[0].__Route << std::endl;
        }
    }
}

void receive_basic(int s, fd_set &current_sockets, int fd_socket,  std::map<int , data> &req, ConfigFile& conf)// s ==> client socket
{
    req[s].client_socket = s;
    req[s].server_socket = fd_socket;
    char chunk[CHUNK_SIZE];
    memset(chunk ,0 , CHUNK_SIZE);
    int size_read = recv(s , chunk , CHUNK_SIZE - 1, 0);
    std::cout << chunk << std::endl;
    if(size_read < 0 )
    {
        std::cout << "error in recv :(" << std::endl;
        exit(EXIT_FAILURE);
    }
    // std::cout << "size read = " << size_read << "byte" << std::endl;
    std::cout  << "lenth ==> " << req[s].lenth  << "size _read === " << size_read  << std::endl;
    

    // parse here
    // CHECK IF POST
    std::stringstream check(chunk);
    std::string tmp;
    int headerLength = 0;
    // std::cout << "socket number " << s << std::endl;
    if(req[s].is_header ==  false) // parsing the header.
    {
        std::getline(check, tmp); // get first line in header
        first_line(tmp, req[s]); // just parse first line
        parsing_header(check, req[s]); // paring all headers
        findServerBlock(req[s], conf);
        // found good server
        // std::cout << "name of file get here ->  " <<  req[s]._fileName  << "  check limit == >  " << req[s].lenth  << " check server block  " << req[s].config_block.__ClientLimit << std::endl;
        if(req[s].config_block.__ClientLimit != 0) // if __ClientLimit equel 0 do nothing
        {
            if(req[s].lenth > req[s].config_block.__ClientLimit)
                throw "413";
        }

        tmp = std::string(chunk);
        headerLength = tmp.find("\r\n\r\n") + 4;
        tmp.erase(0, tmp.find("\r\n\r\n") + 4);
        req[s].size_read_complet = req[s].lenth;
        // std::cout << "body ====> " << "|" << tmp  << "|" << std::endl;
        req[s].is_header = true;
        // here
       // std::cout << "host is ==>  " << req[s].host << std::endl;
       // std::cout << "end of parsing " << std::endl;
    }
   
    
    // else write if POST on file or pipe
    // std::cout << "tmp ========================= >> " << tmp.length() << std::endl;
    //  if(req[s].is_header ==  true && tmp.length() != 0 && req[s].method == "POST") // add delete if need
    //  {
    //      // check limmite size and throw error
    //      std::cout << "tessssssssssssssssssssst" << std::endl;
    //    // req[s].create_file(fd_socket, s);
    //     write(req[s]._fileFd, tmp.c_str(),  tmp.length());
    //     tmp.clear();
    //  }
    //  // req[s].is_header == false 



    if (req[s].is_header == true && req[s].method == "POST"  && (size_read  - headerLength > 0)) // if we recv chunk not in body of header
     {
         
         std::cout << "=============================================================================== ana hnaa ======================================" << std::endl;
          req[s].size_read_complet  -= size_read - headerLength;
          std::cout  << ".size_read_complet ==> " << req[s].size_read_complet << std::endl;
        //  if(req[s]._fileName.length() == 0)
        // {
             std::cout << "create file " << std::endl;
           // req[s].create_file(fd_socket, s);
        // }
        // std::cout << "chunk here -------> " << req[s]._fileName << std::endl;
	    write(req[s]._fileFd, chunk + headerLength, (size_read - headerLength));
         std::cout << "===============================================================================  ======================================" << std::endl;

     }



    // if recv return 0  close connectoin  /// check limite size and close client socket
    // if(size_read == 0)
    // {
    //     // close(s);
    //    // req[s].remove = true; // set remove to remove data from map
    //    // FD_CLR(s, &current_sockets);
    //     std::cout << "close and clear file" << std::endl;
    // }
	usleep(10);
	// std::cout << " ------------------------- chuck --------------------" << std::endl;
   // memset(chunk , 0 , CHUNK_SIZE);	//clear the variable
}

/**
 * The main loop where you receive, parse, execute and send back the response to the client.
 * 
 * @param {fd_savior:pointer} Sockets table of each server.
 * @param {socket_list:pointer} Fd_set table of all servers.
 * @param {servers:number} Number of servers.
 * @param {conf:struct} Struct of servers vector.
 * @return {none}.
 */
void start_server(int *fd_savior, fd_set *socket_list, size_t servers, ConfigFile &conf)
{
    ServerBlock meTest;

    struct timeval tm;
    fd_set read_check;
    fd_set  write_check;
    int client_socket;
    std::map<int , data> request_info; // std::map<client_socket, data struct(contains headers ect)>
   
    tm.tv_sec = 0;
    tm.tv_usec = 10;
    while(1)
    {
        for(int i = 0; i <  servers; i++)
        {
            read_check = socket_list[i];
            write_check =  read_check;
            if(select(FD_SETSIZE, &read_check, &write_check, NULL, &tm) < 0)
				exitMessage(1, "select problem");
            for(size_t j = 0; j < FD_SETSIZE; j++)
            {
                if(FD_ISSET(j, &read_check))
                {
                    data tmp;
                    if (j == fd_savior[i])
                    {
                        client_socket = accept(fd_savior[i], NULL, NULL);
                        if (client_socket < 0 )
							exitMessage(1, "accept error");
                        // if(request_info[client_socket]._fileName.length() == 0)
                        tmp.create_file(fd_savior[i], client_socket);
                        request_info[client_socket] = tmp; // add to map
                        FD_SET(client_socket, &socket_list[i]); // set client socket(return of accept) to set
                        usleep(10);
                    }
                    else
                    {
                        // std::cout << " ----------------------------------- in else in read request -------------------------" << std::endl;
                        // iterate through the map until index i
                       // request_info[j].config_block = conf.__Servers[i]; // change to new logic 
                        // std::cout <<"serverroute ======== ===> " << request_info[j].config_block.__Locations[0].__Route << std::endl;
                        // crea
                        //  request_info[j].create_file(fd_socket, s);
                        receive_basic(j, socket_list[i], fd_savior[i], request_info, conf);
                        // std::cout << " ===>============================== end ================================== " << std::endl;
                    }
                }
                if(FD_ISSET(j, &write_check))
                {
                             // scope for check if you can write in client fd
                    // std::cout << request_info[j].size_read_complet << std::endl;
                    if(request_info[j].size_read_complet == 0) // if we complet Content-Length send responce 
                    {
                        response resp;
                        // std::cout << "send req" << std::endl;
                        try
                        {
                            check_url_path(request_info[j], request_info[j].config_block.__Locations); // check first line error
                            // genrate body response and send it
                            resp.generate_response_header("200", request_info[j]);
                            // resp.generate_response_header();
                            resp.send_response(request_info[j]);
                           // write(j, hello, strlen(hello)); // send
                            
                        }
                        catch(char const* error)
                        {
                            // std::cout << "error" << std::endl;
                            // genrate_body_for_errors(error);
                            // send data
                            //    resp.generate_response_header("200", request_info[j]);
                            resp.generate_response_header(error, request_info[j]);
                            resp.send_response(request_info[j]);
                            std::cerr << error << '\n';
                        }
                        
                        //write(j, hello, strlen(hello)); // send
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

void filterByServer(ConfigFile &conf, std::map<std::pair<std::string, unsigned short>, std::string>&unq)
{
    
	// for(int i = 0; i < conf.__Servers.size(); i++)
	// 	unq[conf.__Servers[i].__Port] = conf.__Servers[i].__Host;
    
    for(int i = 0; i < conf.__Servers.size(); i++)
        {
            std::pair<std::string, unsigned short> tmp_pair(conf.__Servers[i].__Host, conf.__Servers[i].__Port);
            unq[tmp_pair] = conf.__Servers[i].__ServerNames[0];
        }
}

/**
 * Filters the servers by unique host:port then creates server sockets
 * and starts to listen on those sockets
 * 
 * @param {conf:Struct} The structure that holds a vector of Servers struct.
 * @return {none}.
 */
void install_servers(ConfigFile &conf)
{
	//		   @key: Port || @value: Host.
    std::map<std::pair<std::string, unsigned short>, std::string> uniqueServers;

    filterByServer(conf, uniqueServers);
    size_t server_size = uniqueServers.size();
	// number of servers.
    int fd_savior[server_size];
	// fd_set of each server.
    fd_set socket_list[server_size];
    // for(int i  = 0; i < server_size; i++)
    // {
        int i = 0;
        std::cout << "=====================+Creationg of the servers+=======================\n";
        for(std::map<std::pair<std::string, unsigned short>, std::string>::iterator it = uniqueServers.begin(); it != uniqueServers.end(); it++)
        {
            std::cout << "server: " << (*it).first.first << " port: " << (*it).first.second << std::endl;
            prepare_socket((*it).first, fd_savior[i]);
            FD_ZERO(&socket_list[i]);
            FD_SET(fd_savior[i], &socket_list[i]);
            i++;
        }
        std::cout << "total servers : " << i << std::endl;
    // }
    start_server(fd_savior, socket_list, server_size, conf);
}