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
		throw "404";
    return S_ISDIR(path_stat.st_mode);
}

int check_file(std::string &file_name)
{
	struct stat fileStat;
  int exist = stat(file_name.c_str(), &fileStat);
  if(exist != 0)
      return 404;
  if (!(fileStat.st_mode & S_IRUSR))
    return 403;
	return 0;
}

void  auto_index(data &req) // pass class data
{
	std::string tmp_path(req.path);
	tmp_path.erase(0, req.location.__Root.length());
    std::string html_save = "<html>\n<head><title>Index of " + tmp_path + "</title></head>\n<body>\n<h1>Index of " + tmp_path + "</h1><hr><pre><a href=\"../\">../</a>\n";
	tmp_path.clear();
   	std::fstream MyFile;
    MyFile.open("/tmp/autoindex.html", std::ios::out | std::ios::trunc); // create uniq file // now just test
    MyFile << html_save;
    html_save.clear();
    DIR *dir;


	// std::cout << "=======================>>>>>>>>>>>path is ==> "  <<req.path.c_str()  << std::endl;
    dir = opendir(req.path.c_str());
    if (dir == NULL)
        throw "403";
    
    struct dirent *entry;

    struct stat st;
	// problem note : 

    while( (entry= readdir(dir)) != NULL) // read all file in folder
    {
      if(entry->d_name[0] != '.')
      {
		  	
			std::string tmp(entry->d_name);
			std::string name_search = req.path + "/" + tmp;
			if(stat(name_search.c_str(), &st) < 0)
			{
				// std::cout << "file name == >" << entry->d_name << std::endl;
				perror("stat: ");
				// std::cerr << "error in stat" << std::endl;
				exit(1);
			}
			if(S_ISDIR(st.st_mode))
			{
				tmp.append("/");
				name_search.erase(0, req.location.__Root.length());
			}
			// std::cerr << "time -==> " << ctime(&st.st_ctime) << std::endl;
			std::string tmp2(ctime(&st.st_ctime));
			tmp2.erase(tmp2.length()-1); // remove "\n" in ctime(&st.st_ctime)
			if(!(S_ISDIR(st.st_mode)))
				name_search.erase(0, req.location.__Root.length());
			if(tmp.length() < 33)
			{
				if(name_search[0] != '/')
					html_save = "<a href=\"/"+ name_search + "\">" +  tmp  + "</a>";
				else
					html_save = "<a href=\""+ name_search + "\">" +  tmp  + "</a>";
			}
			else
			{
			    if(name_search[0] != '/')
					html_save = "<a href=\"/"+ name_search + "\">" +  tmp  + "</a>";
				else
					html_save = "<a href=\""+ name_search + "\">" +  tmp  + "</a>";
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
    MyFile << "</pre><hr></body>\n</html>\n";
	req.path = "/tmp/autoindex.html";
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
	// std::cout << "rout befor " <<req.location.__Route << std::endl; 
	bool is= false;
	std::istringstream to_split(req.path);
	std::string tmp;
	std::string finalString = "/";
	
	std::vector<Locations>::iterator itB = conf.begin();
	for (; itB != conf.end(); itB++)
	{
		// std::cout << "CURRENT LOCATION: " << (*itB).__Route << std::endl;
		// std::cout << std::boolalpha << ((*itB).__Route[0]) << std::endl;
		if ("/" == (*itB).__Route)
		{
			req.location = (*itB);
			break ;
		}
	}
	
	while (getline(to_split, tmp, '/')) // loop to check location // location /t1/t2/
	{
		if (tmp.size() > 0)
		{
			//std::cout << "line --> " << tmp << std::endl;
			// finalString + tmp;
			std::vector<Locations>::iterator it = conf.begin();

			for (; it != conf.end(); it++)
			{
				// std::cout << ">=====================>>>>>=>FinalString + tmp: " << (finalString + tmp) << "    route: "  <<(*it).__Route << std::endl;
				if ((finalString + tmp) == (*it).__Route)
				{
				// std::cout << "==========================================================================================ana hna help ======================================================\n" ;
					// std::cout << "true string ==> " << (finalString + tmp) << std::endl;
					// std::cout << " true" << std::endl;
					req.location = (*it);
					// std::cout << "=================== location ====================" << std::endl;
					// std::cout << "Final chosen location route: " << req.location.__Route << std::endl;
					// std::cout << "=======================end location =============" << std::endl;
					break ;
				}
				else if (req.extension == (*it).__Route)
				{
					req.root_cgi = (*it).__Root; //save cgi path
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
	// std::cout << "path =========> " << req.path << std::endl;
	
	// 	replace route to root
	
	if(req.location.__Root.length() == 0) // []/t1/t2/t3
	{
		//  /t1/2/t3
		req.location.__Root = "./default/";
		int len;
		if(req.location.__Route.length() > 1)
		{

			len = req.location.__Route.length() - 1;
		}
		else
			len  = 0;
		// std::cout << "len ==> " << len  <<  " route ==> " << req.location.__Route.length() << "route is ==> " << req.location.__Route  << std::endl;
		req.path.replace(0, len,req.location.__Root);
		// std::cout << "path =========> " << req.path << std::endl;
	}
	else
		req.path.replace(0, req.location.__Route.length() - 1, req.location.__Root);

	// std::cout << "last info" << std::endl;
	// std::cout << "final path ===> " << req.path << std::endl;
	// std::cout << "location route ===> " << req.location.__Route << std::endl;
	if(is_dir_and_exist(req.path.c_str()) != 0)
	{
		// std::cout << "======================== "<< std::endl;;l.......................0
		//1. default file
		//2. autoindex
		//3. throw error
		bool tr = true;
		int ret = 0;
		if(req.location.__DefaultFile.length() != 0)
		{
			// default file
			req.path.append(req.location.__DefaultFile);
			if (req.path.find_last_of(".") != std::string::npos)
				req.extension = req.path.substr(req.path.find_last_of("."), req.path.length() - req.path.find_last_of("."));
			// std::cout << "extension is ======================> :" << req.extension << std::endl;
			ret = check_file(req.path);
			// std::cout << "throw is >> " << ret <<std::endl;
			if(ret == 0)
				tr = false;
			if(ret == 404 && req.location.__DirList  == false)
				throw "404";
			else if(ret == 403)
				throw "403";
				
		}
		if (req.location.__DirList  == true && tr == true)
		{
			// autoindex
			// std::cout << "======================== "<< std::endl;
			// std::cout << "active autoindex" << std::endl;
			// std::cout << "======================== "<< std::endl;
			req.extension = ".html";
			auto_index(req); // create file html c++ and change path to name the file html
		}
		if(req.location.__DirList  == false && req.location.__DefaultFile.length() == 0)
			throw "403";
		
	}
	else
	{
		/// if file
		int fd  = open(req.path.c_str(), O_RDONLY);
		int ret = check_file(req.path);
		// std::cout << "return check file is ==> " << ret << std::endl;
		
		if(ret == 404)
			throw "404";
		if(ret == 403)
			throw "403";
		// std::cout << "is safe file you can read from it" << std::endl;
		
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
	// cgi work
	this->status_code = status;
	this->reason_phrase = data_base[status];
	if(req.root_cgi.length() != 0)
	{
		std::cout << "start cgi" << std::endl;
		cgi_generate_response(req);
	}
	else
	{
		std::cout << "normal file" << std::endl;
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
		this->header_resp.append("Connection: Keep-Alive\r\n");
		this->header_resp.append("\r\n");
		std::cout << header_resp << std::endl;
	}
}



void response::send_response(data &req)
{
	std::cout << " ===============================> new path "  <<  req.path.c_str() << std::endl;
	this->fd = open(req.path.c_str(), O_RDONLY);
	write(req.client_socket, header_resp.c_str(), strlen(header_resp.c_str())); // send header first
	// std::cerr << " len ==> "  << this->lenth << std::endl;
	
	if(this->lenth > 0)
	{
		char *buff = new char[this->lenth];
		read(this->fd, buff, this->lenth);
		write(req.client_socket, buff, this->lenth);
		delete[] buff;
	}
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