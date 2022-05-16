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
    std::fstream fd_file; // file stream
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
    }
    void create_file(int fd_socket, int client_socket); // create file 
    ~data()
    {
        fd_file.close();
        remove(_fileName.c_str()); // remove file 
    }
};




#endif