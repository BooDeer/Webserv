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
    if ((save = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		exitMessage(1, "socket error");
    server_info.sin_family = PF_INET;
    server_info.sin_addr.s_addr = inet_addr(pair.first.c_str());
    server_info.sin_port =  htons(pair.second);
    fcntl(save, F_SETFL, O_NONBLOCK);
    int optval = 1;
    if ((setsockopt(save, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int))) == -1)
        exit(1);
    if(bind(save, (sockaddr *)&server_info, sizeof(server_info)) < 0)
		exitMessage(1, "bind error");
    if(listen(save, INT32_MAX) < 0)
		exitMessage(1, "listen error");
}

void findServerBlock(data& req, ConfigFile& conf)
{
    std::vector<ServerBlock>::iterator it;

    it = conf.__Servers.begin();
    bool found = false;
    for (; it != conf.__Servers.end(); it++)
    {
        if("0.0.0.0" == req.ip_server_name)
            req.ip_server_name = "127.0.0.1";
        if ((*it).__Port == req.port && ( (*it).__Host == req.ip_server_name || ((*it).__ServerNames.size() && (*it).__ServerNames[0] == req.ip_server_name)))
        {
            req.config_block = *it;
            found = true;
        }
    }
    if (!found)
    {
        it = conf.__Servers.begin();
        for (; it != conf.__Servers.end(); it++)
        {
            if ((*it).__Port == req.port)
            {
                req.config_block = *it;
                break ;
            }
        }
    }
}




void receive_basic(int s, int fd_socket,  std::map<int , data> &req, ConfigFile& conf)// s ==> client socket
{
    char chunk[CHUNK_SIZE];
	int size_read;
    std::string tmp;

    req[s].client_socket = s;
    req[s].server_socket = fd_socket;
    memset(chunk ,0 , CHUNK_SIZE);
    size_read = recv(s , chunk , CHUNK_SIZE - 1, 0);
    if(size_read < 0 )
    {
        req[s].status_code = "500";
        req[s].remove = true;
        return;
    }
    else if(size_read == 0)
    {
        req[s].remove = true;
        return ;
    }

    std::stringstream check(chunk);
    int headerLength = 0;
    if(req[s].is_header ==  false) // parsing the header.
    {
        std::getline(check, tmp);
         first_line(tmp, req[s]);
        parsing_header(check, req[s]);
        findServerBlock(req[s], conf);
        if(req[s].config_block.__ClientLimit != 0) // if __ClientLimit equel 0 do nothing
        {
            if(req[s].lenth > req[s].config_block.__ClientLimit)
                req[s].status_code = "413";
            return;
        }
        tmp = std::string(chunk);
        headerLength = tmp.find("\r\n\r\n") + 4;
        tmp.erase(0, tmp.find("\r\n\r\n") + 4);
        req[s].size_read_complet = req[s].lenth;
        req[s].is_header = true;
    }
    if (req[s].is_header == true && req[s].method == "POST"  && (size_read  - headerLength > 0)) // if we recv chunk not in body of header
     {
        req[s].size_read_complet  -= size_read - headerLength;
	    int ret = write(req[s]._fileFd, chunk + headerLength, (size_read - headerLength));
        if(ret == 0)
            ;
        else if(ret == -1)
            req[s].status_code = "500";
     }
}

void check_metod(data &d)
{
     const char *save = d.method.c_str();
    for(size_t i = 0; save[i] != '\0'; i++)
    {
        if(islower(save[i]))
        {
            if( d.config_block.__DefaultErrorpg.size() == 0)
					d.extension = ".html"; 
            throw "400";
        }
    }
    // GET POST DELETE
    if(d.method.length() != 0)
    {
        if(d.method != "GET" && d.method != "POST" && d.method != "DELETE")
        {
            {
                if(d.config_block.__DefaultErrorpg.size() == 0)
					d.extension = ".html"; 
                throw "405";
            }
        }
    }
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
    FD_ZERO(&write_check);
    FD_ZERO(&read_check);
    int client_socket;
    std::map<int , data> request_info; // std::map<client_socket, data struct(contains headers ect)>
   
    tm.tv_sec = 0;
    tm.tv_usec = 10;
    while(1)
    {
        for(size_t i = 0; i <  servers; i++)
        {
            read_check = socket_list[i];
            FD_ZERO(&write_check);
            if(select(FD_SETSIZE, &read_check, &write_check, NULL, &tm) < 0)
				exitMessage(1, "select problem");
            for(int j = 0; j < FD_SETSIZE; j++)
            {
                if(FD_ISSET(j, &read_check))
                {
                    data tmp;
                    if (j == fd_savior[i] && request_info.size() < FD_SETSIZE)
                    {
                        client_socket = accept(fd_savior[i], NULL, NULL);
                       fcntl(client_socket, F_SETFL, O_NONBLOCK);
                        if (client_socket < 0 )
							exitMessage(1, "accept error");
                        tmp.create_file(fd_savior[i], client_socket);
                        request_info[client_socket] = tmp; // add to map
                        FD_SET(client_socket, &socket_list[i]);
                        usleep(10);
                    }
                    else if(request_info.find(j) != request_info.end())
                    {
                        receive_basic(j, fd_savior[i], request_info, conf);
                        write_check =  read_check; // 
                    }
                }
                if(FD_ISSET(j, &write_check))
                {
                    if(request_info.find(j) != request_info.end() && request_info[j].size_read_complet == 0) // if we complet Content-Length send responce 
                    {
                        response resp;
                        if(request_info[j].remove == false)
                        {
                            try
                            {
                                if(request_info[j].status_code.length() != 0)
                                    throw request_info[j].status_code.c_str();
                                check_metod(request_info[j]); // check method 
                                check_url_path(request_info[j], request_info[j].config_block.__Locations);
                                // generate body response and send it
                                resp.generate_response_header("200", request_info[j]);
                                resp.send_response(request_info[j]);
                                
                            }
                            catch(char const* error)  
                            {
                                request_info[j].root_cgi.clear(); // for error in .php
                                if(request_info[j].config_block.__DefaultErrorpg.size() == 0)
                                    request_info[j].extension = ".html"; 
                                resp.generate_response_header(error, request_info[j]);
                                resp.send_response(request_info[j]);
                            }
                        }
                        FD_CLR(j, &socket_list[i]);
                        close(j);
                        close(request_info[j]._fileFd);
                        remove(request_info[j]._fileName.c_str());
                        if(resp.cgi_remove.length() != 0)
                            remove(resp.cgi_remove.c_str());
                        if(resp.output_file_name.length() != 0)
                            remove(resp.output_file_name.c_str());
                        request_info.erase(j);
                    }
                }
            }
        }
    }
}

void filterByServer(ConfigFile &conf, std::map<std::pair<std::string, unsigned short>, std::string>&unq)
{

    for(size_t i = 0; i < conf.__Servers.size(); i++)
    {
        std::pair<std::string, unsigned short> tmp_pair(conf.__Servers[i].__Host, conf.__Servers[i].__Port);
        if (conf.__Servers[i].__ServerNames.size())
            unq[tmp_pair] = conf.__Servers[i].__ServerNames[0];
        else
            unq[tmp_pair];
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
    int i = 0;
    for(std::map<std::pair<std::string, unsigned short>, std::string>::iterator it = uniqueServers.begin(); it != uniqueServers.end(); it++)
    {
        prepare_socket((*it).first, fd_savior[i]);
        FD_ZERO(&socket_list[i]);
        FD_SET(fd_savior[i], &socket_list[i]);
        i++;
    }
    start_server(fd_savior, socket_list, server_size, conf);
}