#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>


// void	ft_split(std::string const& line, std::vector<std::string> &dst)
// {
// 	std::istringstream iss(src);
	
// 	do
// 	{
// 		std::string subs;
// 		iss >> subs;
// 		if (subs.size() > 0)
// 		dst.push_back(subs);
// 	} while (iss);
// }


struct data
{
    int id; // id for request
    std::string path;
    std::string method;
    std::string host;
    std::string lenth;
    std::string referer;
    data()
    {
        id = -666;
    }
};
// template<class FILE>
void first_line(std::string line, data &save) // ====> GET example.com HTTP/1.1
{
    // data save;
    std::istringstream iss(line);
	std::string     buff;
    // std::cout << line << std::endl;
    for(int i = 0; i < 3; i++)
    {
        iss >> buff;
        if(i == 0)
            save.method = buff;
        else if(i == 1)
            save.path = buff;
    }
    // std::cout << save.method << std::endl << save.path << std::endl;
}

// 
// GET /// file / versiohttp
/* 


4
requset_4
 */

void parsing_header(std::fstream &fs, data &d)
{
    // find host  Referer lenth
    std::string lines;
    while (std::getline(fs, lines))
    {
        if (lines.find("Host:") != std::string::npos) // Host: 127.0.0.1:2082
        {
            // std::cout << "Reached here ==> "  << lines << std::endl;
            d.host = lines.erase(0, strlen("Host: "));
            // std::cout << "line == >" << lines << std::endl;
        }
        else if (lines.find("Content-Length:") != std::string::npos) // Content-Length: 69
        {
            // std::cout << "Reached here ==> "  << lines << std::endl;
            // lines.erase(0, strlen("Content-Length: "));
            d.lenth =  lines.erase(0, strlen("Content-Length: "));
            // std::cout << "line == >" << lines << std::endl; 
        }
        else if (lines.find("Referer:") != std::string::npos) // Referer: http://127.0.0.1:2082/
        {
            // std::cout << "Reached here ==> "  << lines << std::endl;
            // lines.erase(0, strlen("Content-Length: "));
            d.referer = lines.erase(0, strlen("Referer: "));
            // std::cout << "line == >" << lines << std::endl; 
        }
        else if (lines.find("\r\n\r\n") != std::string::npos) // To be checked
            break ;
    }
}




int main() {

    std::fstream fs;
    data data_to_save;
    // fs.open ("ex_3", std::fstream::in | std::fstream::out | std::fstream::app);
    fs.open ("ex_3", std::fstream::in | std::fstream::out);
    std::string line_save;
   // fs << "\r\n\r\n";
    std::getline(fs, line_save);
    first_line(line_save, data_to_save);
    parsing_header(fs, data_to_save); // Parsing the rest of the request. (Header)
    // getline(f, s, ';');
    // std:
}