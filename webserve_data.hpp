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
        struct timeval	time;
		long long t;
        gettimeofday(&time, NULL);
        t = (time.tv_usec / 1000) + (time.tv_sec * 1000);
        id = -666; // hehe
        std::stringstream save, save2, time222;
        save << client_socket;
        std::string id_c;
        save >> id_c;
        save2 << server_socket;
        std::string id_s;
        save2 >> id_s;
        time222 << t;
        std::string time_;
        time222 >> time_;
        _fileName = std::string("/tmp/webServ_" + id_c + "_" + id_s + time_ +".tmp");
        // std::string file_name("thisisatest");
        fd_file.open(_fileName, std::fstream::out );
        std::cout << _fileName << std::endl;
    }
    ~data()
    {
        fd_file.close();
        remove(_fileName.c_str());
    }
};




#endif