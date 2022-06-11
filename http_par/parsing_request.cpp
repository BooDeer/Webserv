#include "parsing.hpp"

void first_line(std::string line, data &save)
{
    std::istringstream iss(line);
	std::string     buff;
    std::string delimiter = "&";
    size_t d;

    for(int i = 0; i < 3; i++)
    {
        iss >> buff;
        if(i == 0)
            save.method = buff;
        else if(i == 1)
            save.path = buff;
    }
    std::string tmp(save.path);
    if ((d =save.path.find("?")) != std::string::npos)
    {
        tmp.erase(0, d+1);
        save.path.erase(d, save.path.size() - d);
    }
    std::cout << "path from parssing " << save.path << std::endl; 
    save.paramter =  tmp;
    std::string tmp2(save.path);
    size_t pt;
    if ((pt = tmp2.find(".")) != std::string::npos)
    {
        tmp2 = tmp2.substr(pt, tmp2.size() - pt);
        save.extension = tmp2;
    }
}

void parsing_header(std::stringstream &fs, data &d)
{
    std::string lines;

    while (std::getline(fs, lines))
    {
        if (lines.find("Host:") != std::string::npos)
        {
            d.host = lines.erase(0, strlen("Host: "));
            size_t test;
            if ((test = d.host.find(":")) != std::string::npos)
            {
                 std::stringstream ss; 
                 ss << d.host.substr(test+1);
                ss >> d.port;
                d.ip_server_name = d.host.substr(0, test);
            }
        }
        else if (lines.find("Content-Length:") != std::string::npos)
        {
            std::string save = lines.erase(0, strlen("Content-Length: "));
            std::istringstream(save) >> d.lenth;
        }
       else if (lines.find("Content-Type:") != std::string::npos)
        {
            d.type =  lines.erase(0, strlen("Content-Type: "));
            d.type.erase(d.type.find_last_of("\r"));
        }
         else if (lines.find("Cookie:") != std::string::npos)
        {
            d.Cookie =  lines.erase(0, strlen("Cookie: "));
            d.Cookie.erase(d.Cookie.find_last_of("\r"));
        }
        else if (lines.find("Referer:") != std::string::npos)
            d.referer = lines.erase(0, strlen("Referer: "));
        if (lines.length() == 1)
            break;
    }
}
