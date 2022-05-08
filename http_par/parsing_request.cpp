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
    std::string path;
    std::string method;
};
// template<class FILE>
void first_line(std::string line) // ====> GET example.com HTTP/1.1
{
    data save;
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
    std::cout << save.method << std::endl << save.path << std::endl;
}

// 
// GET /// file / versiohttp
/* 


4
requset_4
 */

// using namespace std;

int main() {
    // std::vector<std::string> strings;
    // std::istringstream f(";;test;;");
    // std::string s;    
    // while (std::getline(f, s, ';')) {
    //     if (s.length() > 0)
    //     {
    //         std::cout << s << std::endl;
    //         strings.push_back(s);
    //     }
    // }
      std::fstream fs;
    
        fs.open ("ex_3", std::fstream::in | std::fstream::out);
        std::string line_save;
        std::getline(fs, line_save);
        first_line(line_save);
    // getline(f, s, ';');
    // std:
}