#ifndef WEBSERVE_DATA_HPP
#define WEBSERVE_DATA_HPP

#include <iostream>
#include <iostream>
#include <vector>
#include <sys/time.h>
#include <sstream>
#include <fstream>
#include "webserv.hpp"

struct data
{
    int id; // id for request
   // std::ofstream fd_file; // file stream
	int	_fileFd;
    bool is_header; // check is header
    bool remove; // remove from header
    std::string _fileName;
    int client_socket; // client id socket 
    int server_socket; //  server is socket
    std::string path;
    std::string method;
    std::string host;
    unsigned long long lenth;
    std::string type;
    std::string referer;
    int status_code;
    std::vector<std::string> paramter;
    std::string extension;
    ServerBlock config_block; // server block config
    
    public:
    data()
    {
        this->id  = 0;
        this->client_socket = 0;
        this->server_socket = 0;
        this->status_code = 0;
        this->lenth = 0;
        is_header = false;
        remove = false;
    }
    data& operator=(data &o)
    {
        std::cout << "---------------operator =" << std::endl;
        this->id = o.id;
        this->_fileName = o._fileName;
		this->_fileFd = open(o._fileName.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
        this->client_socket = o.client_socket;
        this->server_socket = o.server_socket;
        this->path = o.path;
        this->method = o.method;
        this->host = o.host;
        this->lenth = o.lenth;
        this->referer = o.referer;
        this->status_code = o.status_code;
        this->paramter = o.paramter;
        this->extension = o.extension;
		return *this;
    }
    void create_file(int fd_socket, int client_socket); // create file 
    ~data()
    {
         this->id  = 0;
        this->client_socket = 0;
        this->server_socket = 0;
        this->status_code = 0;
        is_header = false;
        remove = false;
      //  close(_fileFd);
    }
    
};




#endif