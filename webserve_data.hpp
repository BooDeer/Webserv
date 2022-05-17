#ifndef WEBSERVE_DATA_HPP
#define WEBSERVE_DATA_HPP

#include <iostream>
#include <iostream>
#include <vector>
#include <sys/time.h>
#include <sstream>
#include <fstream>

struct data
{
    int id; // id for request
    std::ofstream fd_file; // file stream
    std::string _fileName;
    int client_socket;
    int server_socket;
    std::string path;
    std::string method;
    std::string host;
    std::string lenth;
    std::string referer;
    int status_code;
    std::vector<std::string> paramter;
    std::string extension;
    data()
    {
        this->id  = 0;
        this->client_socket = 0;
        this->server_socket = 0;
        this->status_code = 0;
        // this->fd_file = new std::ofstream();
    }
    void operator=(data &o)
    {
        std::cout << "---------------operator =" << std::endl;
        this->id = o.id;
        this->_fileName = o._fileName;
        this->fd_file.open(_fileName); // open file again with name
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
    }
    void create_file(int fd_socket, int client_socket); // create file 
    ~data()
    {
       // fd_file->close();
        //remove(_fileName.c_str()); // remove file 
    }
    
};




#endif