#ifndef WEBSERVE_DATA_HPP
#define WEBSERVE_DATA_HPP

#include <iostream>
#include <iostream>
#include <vector>
#include <fstream>

struct data
{
    int id; // id for request
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
        id = -666; // hehe
    }
};




#endif