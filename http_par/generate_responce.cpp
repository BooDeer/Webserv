#include "../webserv.hpp"
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <iomanip>
#include <ctime>
int is_dir_and_exist(const char *path)
{
    struct stat path_stat;
    int exist = stat(path, &path_stat);
	if(exist != 0)
		throw 404;
    return S_ISDIR(path_stat.st_mode);
}

void  auto_index(data &req) // pass class data
{
    std::string html_save = "<html>\n<head><title>Index of " + req.path + "</title></head>\n<body>\n<h1>Index of " + req.path + "</h1><hr><pre><a href=\"../\">../</a>\n";
   std::fstream MyFile;
    MyFile.open("/tmp/autoindex.html", std::ios::out); // create uniq file // now just test
    MyFile << html_save;
    html_save.clear();
    DIR *dir;

    dir = opendir("./");
    if (dir == NULL)
        throw "403";
    
    struct dirent *entry;

    struct stat st;

    while( (entry= readdir(dir)) != NULL) // read all file in folder
    {
      if(entry->d_name[0] != '.')
      {
			std::string tmp(entry->d_name);
			stat(entry->d_name, &st);
			std::string tmp2(ctime(&st.st_ctime));
			tmp2.erase(tmp2.length()-1); // remove "\n" in ctime(&st.st_ctime)
			if(tmp.length() < 33)
			  html_save = "<a href=\""+ tmp + "\">" +  tmp  + "</a>";
			else
			{
			    html_save = "<a href=\""+ tmp + "\">";
			    for(int i = 0; i <  33; i++)
			    {
			      html_save.append(1, tmp[i]);
			    }
			     html_save.append("..&gt;</a>");
			}
			MyFile << html_save           <<  std::setw(60 - tmp.length())  << tmp2 << std::setw(10)  << st.st_size << std::endl;
			tmp.clear();
			tmp2.clear();
			html_save.clear();
      }

    }
	req.path = "/tmp/autoindex.html";
    MyFile << "</pre><hr></body>\n</html>\n";
	MyFile.close();
    closedir(dir);
}

//! Should have a SeverBlock parametre instead of locations.
//TODO: iterate through the locations vector instead of loop.
void check_url_path(data &req, std::vector<Locations> &conf) // check url ==> GET /index.html http
{
	// 1 - location block == > /test/test/test/index.html
	// locations /test/test
	// 2 - file exists :(
	// 3 - file's permissions
	// 4 - file's extension
	std::cout << "rout befor " <<req.location.__Route << std::endl; 
	bool is= false;
	std::istringstream to_split(req.path);
	std::string tmp;
	std::string finalString = "/";
	
	while (getline(to_split, tmp, '/')) // loop to check location // location /t1/t2/
	{
		if (tmp.size() > 0)
		{
			//std::cout << "line --> " << tmp << std::endl;
			// finalString + tmp;
			std::vector<Locations>::iterator it = conf.begin();
			for (; it != conf.end(); it++)
			{
				if ((finalString + tmp) == (*it).__Route)
				{
					// std::cout << "true string ==> " << (finalString + tmp) << std::endl;
					// std::cout << " true" << std::endl;
					req.location = (*it);
					break ;
				}
			} 
			finalString.append(tmp + "/");
			// std::cout << " final test ==> " << finalString << std::endl;
		}
	}
	// std::cout << "is ==> " << finalString  << "|" << std::endl;
	if(finalString.length() > 0)
	{
		finalString.erase(finalString.length() - 1, 1); // last /
		finalString.erase(0, 1);
	}
	// t1/t2/t1/t2
	//std::cout << " final string is  ==> " << finalString << std::endl; 
	req.path.clear();
	req.path = finalString; // path final ex ==> T1/T2/T3
	std::cout << "path =========> " << req.path << std::endl;
	
	// 	replace route to root

	if(req.location.__Root.length() == 0) // []/t1/t2/t3
	{
		//  /t1/2/t3
		int len;
		if(req.location.__Route.length() > 1)
		{

			len = req.location.__Route.length() - 1;
		}
		else
			len  = 0;
		std::cout << "len ==> " << len  <<  " route ==> " << req.location.__Route.length() << "route is ==> " << req.location.__Route  << std::endl;
		req.path.replace(0, len, "./default/");
		// std::cout << "path =========> " << req.path << std::endl;
	}
	else
		req.path.replace(0, req.location.__Route.length() - 1, req.location.__Root);

	std::cout << "last info" << std::endl;
	std::cout << "final path ===> " << req.path << std::endl;
	std::cout << "location route ===> " << req.location.__Route << std::endl;
	if(is_dir_and_exist(req.path.c_str()) != 0)
	{
		std::cout << "is dir" << std::endl;
		//1. default file
		//2. autoindex
		//3. throw error
		if(req.location.__DefaultFile.length() != 0)
		{
			// default file
			
		}
		else if (req.location.__DirList  == true)
		{
			// autoindex
			auto_index(req); // create file html c++ and change path to name the file html
		}
		else
			throw 403;
	}
	else
	{
		int fd  = open(req.path.c_str(), O_RDONLY);
		if(fd < 0)
		{
			std::cout << "here " << std::endl;
			close(fd);
			throw 403;
		}
		std::cout << "is safe file you can read from it" << std::endl;
		
	}
	/* 
		url: ./default/T3

		location:route T1/T2 
	
	 */
	// std::cout << "The final string: " << finalString << std::endl;
	// is dir

}


void response::generate_response_header(const std::string &status, data &req)
{
	this->status_code = status;
	this->reason_phrase = data_base[status];
	this->header_resp = "HTTP/1.1 " + status_code + " " + reason_phrase + "\r\n"; // first line for response
	this->header_resp.append("Server: webserv\r\n");
	this->header_resp.append("Content-Length: ");
	std::ifstream in(req.path.c_str(), std::ifstream::ate | std::ifstream::binary);
	long long a = in.tellg();
	std::stringstream b;
	b << a;
	std::string tmp;
	b >> tmp;
	this->lenth = a;
	this->header_resp.append(tmp);
	this->header_resp.append("\r\n");
	this->header_resp.append("Content-Type: ");
	if(Common_types.find(req.extension) != Common_types.end())
		this->header_resp.append(Common_types[req.extension]);
	else
		this->header_resp.append("application/octet-stream");
	this->header_resp.append("\r\n");
	this->header_resp.append("\r\n");
	std::cout << header_resp << std::endl;
}

void response::send_response(data &req)
{
	this->fd = open(req.path.c_str(), O_RDONLY);
	write(req.client_socket, header_resp.c_str(), strlen(header_resp.c_str())); // send header first
	char *buff = new char[this->lenth];

	read(this->fd, buff, this->lenth);
	write(req.client_socket, buff, this->lenth);
	delete[] buff;
}

// int main()
// {
// 	response a;
// 	data req;

// 	req.extension = ".html";
// 	req.path = "boodTest.cpp";
// 	a.generate_response_header("200", req);
// 	// /test/test/test/test/
// }