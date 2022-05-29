#include "execution.hpp"
#include "../webserv.hpp"

#define CHUNK_SIZE 10000

/**
 * Creates the server socket, bind it, sets it to non-blocking and starts listening.
 * 
 * @param {it:string} Host (e.g: 127.0.0.1).
 * @param {save:number} Server's socket.
 * @return {lol}.
 */
void prepare_socket(std::map<unsigned short, std::string>::iterator& it, int &save)
{
    
    struct sockaddr_in server_info;

    std::cout << "------------crearte socket----------- with port "  <<  (*it).first << std::endl;
    if ((save = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		exitMessage(1, "socket error");
    server_info.sin_family = PF_INET;
    server_info.sin_addr.s_addr = inet_addr((*it).second.c_str());
    server_info.sin_port =  htons((*it).first);
    fcntl(save, F_SETFL, O_NONBLOCK);
    if(bind(save, (sockaddr *)&server_info, sizeof(server_info)) < 0)
		exitMessage(1, "bind error");
    if(listen(save, INT32_MAX) < 0)
		exitMessage(1, "listen error");
}

void receive_basic(int s, fd_set &current_sockets, int fd_socket,  std::map<int , data> &req)// s ==> client socket
{
    req[s].client_socket = s;
    req[s].server_socket = fd_socket;
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
     else if (req[s].is_header == true && req[s].method == "POST") // if we recv chunk not in body of header
     {
        std::cout << "chunk here -------> " << req[s]._fileName << std::endl;
	    write(req[s]._fileFd, chunk, std::strlen(chunk));
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
	std::cout << " ------------------------- chuck --------------------" << std::endl;
    memset(chunk , 0 , CHUNK_SIZE);	//clear the variable
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
    struct timeval tm;
    fd_set read_check;
    fd_set  write_check;
    int client_socket;
    std::map<int , data> request_info; // std::map<client_socket, data struct(contains headers ect)>
   
    tm.tv_sec = 0;
    tm.tv_usec = 10;
    char hello[82] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 20\n\n<h1>Hello world!</h1>";
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
                    std::cout << " j is set == > " << j << std::endl;
                    data tmp;
                    if (j == fd_savior[i])
                    {
                        client_socket = accept(fd_savior[i], NULL, NULL);
                        if (client_socket < 0 )
							exitMessage(1, "accept error");
                        request_info[client_socket] = tmp; // add to map
                        FD_SET(client_socket, &socket_list[i]); // set client socket(return of accept) to set
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
                        response resp;
                        std::cout << "send req" << std::endl;
                        try
                        {
                            check_url_path(request_info[j], request_info[j].config_block.__Locations); // check first line error
                            // genrate body response and send it
                            resp.generate_response_header("200", request_info[j]);
                            // resp.generate_response_header();
                            resp.send_response(request_info[j]);
                           // write(j, hello, strlen(hello)); // send
                            
                        }
                        catch(int error)
                        {
                            std::cout << "error" << std::endl;
                            // genrate_body_for_errors(error);
                            // send data
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

void filterByServer(ConfigFile &conf, std::map<unsigned short, std::string> &unq)
{
	for(int i = 0; i < conf.__Servers.size(); i++)
		unq[conf.__Servers[i].__Port] = conf.__Servers[i].__Host;
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
    std::map<unsigned short, std::string> uniqueServers;

    filterByServer(conf, uniqueServers);
    size_t server_size = uniqueServers.size();
	
	// number of servers.
    int fd_savior[server_size];
	// fd_set of each server.
    fd_set socket_list[server_size];
    std::map<unsigned short, std::string>::iterator it2 =  uniqueServers.begin();
    for(int i  = 0; i < server_size; i++)
    {
        prepare_socket(it2, fd_savior[i]);
        FD_ZERO(&socket_list[i]);
        FD_SET(fd_savior[i], &socket_list[i]);
        it2++;
    }
   start_server(fd_savior, socket_list, server_size, conf);
}
