#include "../webserv.hpp"
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <iomanip>
#include <ctime>
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <ftw.h>
#include <unistd.h>
int is_dir_and_exist(const char *path)
{
	struct stat path_stat;
	int exist = stat(path, &path_stat);
	if (exist != 0)
		throw "404";
	return S_ISDIR(path_stat.st_mode);
}

int check_file(std::string &file_name)
{
	struct stat fileStat;
	int exist = stat(file_name.c_str(), &fileStat);

	if (exist != 0)
		return 404;
	if (!(fileStat.st_mode & S_IRUSR))
		return 403;
	return 0;
}

void errors::generate_error(response &resp, std::map<std::string, std::string> &error)
{
	bool is_exist = false;
	struct stat path_stat;
	int exist = stat(error[resp.status_code].c_str(), &path_stat);
	if (exist == 0)
	{
		if (path_stat.st_mode & S_IFREG)
			is_exist = true;
	}
	if (this->you_can_genrate == true)
	{
		resp.header_resp.append("<html>\n<head><title>");
		resp.header_resp.append(resp.status_code);
		resp.header_resp.append(" ");
		resp.header_resp.append(this->phrase);
		resp.header_resp.append("</title></head>\n<body>");
		resp.header_resp.append("<center><h1>");
		resp.header_resp.append(resp.status_code);
		resp.header_resp.append(" ");
		resp.header_resp.append(this->phrase);
		resp.header_resp.append("</center></h1>");
		resp.header_resp.append("</body>\n</html>");
	}
}
void auto_index(data &req)
{
	std::string tmp_path(req.path);
	tmp_path.erase(0, req.location.__Root.length());
	std::string html_save = "<html>\n<head><title>Index of " + tmp_path + "</title></head>\n<body>\n<h1>Index of " + tmp_path + "</h1><hr><pre><a href=\"../\">../</a>\n";
	tmp_path.clear();
	std::fstream MyFile;
	MyFile.open("/tmp/autoindex.html", std::ios::out | std::ios::trunc);
	MyFile << html_save;
	html_save.clear();
	DIR *dir;

	dir = opendir(req.path.c_str());
	if (dir == NULL)
		throw "403";

	struct dirent *entry;

	struct stat st;

	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_name[0] != '.')
		{
			std::string tmp(entry->d_name);
			std::string name_search = req.path + "/" + tmp;
			if (stat(name_search.c_str(), &st) < 0)
			{
				perror("stat: "); //TODO: Forbidden errno usage.
				exit(1);
			}
			if (S_ISDIR(st.st_mode))
			{
				tmp.append("/");
				name_search.erase(0, req.location.__Root.length());
			}
			std::string tmp2(ctime(&st.st_ctime));
			tmp2.erase(tmp2.length() - 1);
			if (!(S_ISDIR(st.st_mode)))
				name_search.erase(0, req.location.__Root.length());
			if (tmp.length() < 33)
			{
				if (name_search[0] != '/')
					html_save = "<a href=\"/" + name_search + "\">" + tmp + "</a>";
				else
					html_save = "<a href=\"" + name_search + "\">" + tmp + "</a>";
			}
			else
			{
				if (name_search[0] != '/')
					html_save = "<a href=\"/" + name_search + "\">" + tmp + "</a>";
				else
					html_save = "<a href=\"" + name_search + "\">" + tmp + "</a>";
				for (int i = 0; i < 33; i++)
					html_save.append(1, tmp[i]);
				html_save.append("..&gt;</a>");
			}
			MyFile << html_save << std::setw(60 - tmp.length()) << tmp2 << std::setw(10) << st.st_size << std::endl;
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
int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
	struct stat fileStat;
	stat(fpath, &fileStat);

	if(fileStat.st_mode & S_IWUSR)
		remove(fpath);
	else
		throw "403";
    return 0;
}

int rmrf(const char *path)
{
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}
void delete_handling(data &req)
{
    // 1. file is exist
    // 2. check perms
    // 3. is directory (delete using tree)
    struct stat fileStat;
    std::cout << "path u clear   " << req.path << std::endl;
    int exist = stat(req.path.c_str(), &fileStat);
    if(exist == 0)
    {
        if(!(S_ISDIR(fileStat.st_mode)) && (fileStat.st_mode & S_IWUSR)) //
        {
            remove(req.path.c_str());
            throw "200";
        }
        else if((!(fileStat.st_mode & S_IRUSR)))
            throw "403"; // if not have permassion
        else if(S_ISDIR(fileStat.st_mode) && (fileStat.st_mode & S_IWUSR)) // 
        {
            rmrf(req.path.c_str());
            throw "200";
        }
    }
    else
        throw "404"; // if file not found


}
void check_url_path(data &req, std::vector<Locations> &conf)
{
	// bool is = false;
	std::istringstream to_split(req.path);
	std::string tmp;
	std::string finalString;

	std::vector<Locations>::iterator itB = conf.begin();
	for (; itB != conf.end(); itB++)
	{
		if ("/" == (*itB).__Route)
		{
			req.location = (*itB);
			break;
		}
	}
	while (getline(to_split, tmp, '/'))
	{
		if (tmp.size() > 0)
		{
			std::vector<Locations>::iterator it = conf.begin();
			for (; it != conf.end(); it++)
			{
				if ((finalString + tmp) == (*it).__Route)
				{
					req.location = (*it);
					break;
				}
				else if (req.extension == (*it).__Route)
				{
					req.root_cgi = (*it).__Root;
				}
			}
			finalString.append(tmp + "/");
		}
	}
	//std::cout << "route=== " << req.location.__Route << "root" << req.location.__Root << std::endl;
	if (req.location.__Redirection[0].length() != 0)
		throw req.location.__Redirection[0].c_str();

	if (req.location.__Root.length() == 0)
	{
		req.location.__Root = "./default";
		// if (req.location.__Route.length() > 1)
		// {
		std::cout << "route is ====> " << req.location.__Route << std::endl;
		req.path.replace(0, 0, req.location.__Root);
		std::cout << "after replace ====> " << req.location.__Route << std::endl;
		std::cout <<  "path befor replace  ==> "  << req.path <<  std::endl; 
	}
	else
		req.path.replace(0, req.location.__Route.length() - 1, req.location.__Root);
	std::cout << "path == > " <<  req.path << std::endl;
	 if(req.method == "DELETE")
     {
         std::cout << "start delete" << std::endl;
        delete_handling(req);
    }
	if (is_dir_and_exist(req.path.c_str()) != 0)
	{
		if(req.path[req.path.length() - 1] != '/')
		{
			std::stringstream a;
			a << req.port;
			std::string tmp_port;
			a >> tmp_port;
			req.location.__Redirection[0] = "301";
			std::cout << "lol ==> " << req.path << std::endl;
			req.location.__Redirection[1] = req.path.erase(0, req.location.__Root.length()) + "/";
			std::cout << "redoraction " << req.location.__Redirection[1] << std::endl;
			throw "301";
		}
		std::cout << "test  path ===> " << req.path  << std::endl;
		bool tr = true;
		int ret = 0;
		std::cout << "default ============= " << req.location.__DefaultFile  << std::endl;
		if (req.location.__DefaultFile.length() != 0)
		{
			req.path.append(req.location.__DefaultFile);
			if (req.path.find_last_of(".") != std::string::npos)
				req.extension = req.path.substr(req.path.find_last_of("."), req.path.length() - req.path.find_last_of("."));
			ret = check_file(req.path);
			if (ret == 0)
				tr = false;
			if (ret == 404 && req.location.__DirList == false)
				throw "404";
			else if (ret == 403)
				throw "403";
		}
		if (req.location.__DirList == true && tr == true)
		{
			req.extension = ".html";
			auto_index(req);
		}
		if (req.location.__DirList == false && req.location.__DefaultFile.length() == 0)
			throw "403";
	}
	else
	{
		int ret = check_file(req.path);

		if (ret == 404)
			throw "404";
		if (ret == 403)
			throw "403";
	}
}

void response::redirection_header_generate(data &req)
{

	this->status_code = req.location.__Redirection[0];
	this->header_resp.clear();
	std::map<std::string, errors>::iterator it = data_base.find(this->status_code);
	
	if(it != data_base.end() && req.location.__Redirection[0][0] == '3')
	{
		this->reason_phrase = data_base[this->status_code].phrase;
		this->header_resp = "HTTP/1.1 " + status_code + " " + reason_phrase + "\r\n";
		this->header_resp.append("Server: webserv\r\n");
		this->header_resp.append("Location: ");
		this->header_resp.append(req.location.__Redirection[1]);
		this->header_resp.append("\r\n\r\n");
	}
	else
	{
		 this->reason_phrase.clear();
		this->header_resp = "HTTP/1.1 " + status_code + " " + this->reason_phrase +"\r\n";
		this->header_resp.append("Server: webserv\r\n");
		this->header_resp.append("Content-Type: application/octet-stream");
		this->header_resp.append("\r\n\r\n");
		this->header_resp.append(req.location.__Redirection[1]);
	}
}

void response::generate_response_header(const std::string &status, data &req)
{
	this->status_code = status;
	if(req.location.__Redirection[0].length() != 0)
	{
		redirection_header_generate(req);
		return;
	}

	this->reason_phrase = data_base[this->status_code].phrase;
	if (req.root_cgi.length() != 0)
	{
		cgi_generate_response(req);
		req.path = output_file_name;
		data_base[this->status_code].you_can_genrate = true;
	}
	else
	{
		this->header_resp = "HTTP/1.1 " + status_code + " " + reason_phrase + "\r\n";
		this->header_resp.append("Server: webserv\r\n");
		struct stat fileStat;
		int exist = stat(req.path.c_str(), &fileStat);
		std::string tmp;
		if (exist == 0 && !S_ISDIR(fileStat.st_mode))
		{
			std::stringstream b;
			unsigned long long a = fileStat.st_size;
			b << a;
			b >> tmp;
			this->lenth = a;
			this->header_resp.append("Content-Length: ");
			this->header_resp.append(tmp);
			this->header_resp.append("\r\n");
		}
		else
		{
			if (req.extension.length() == 0)
				req.extension = ".html";
			this->lenth = 0;
		}

		this->header_resp.append("Content-Type: ");
		if (Common_types.find(req.extension) != Common_types.end())
			this->header_resp.append(Common_types[req.extension]);
		else
			this->header_resp.append("application/octet-stream");
		this->header_resp.append("\r\n");
		this->header_resp.append("Connection: Keep-Alive\r\n");
		this->header_resp.append("\r\n");
	}
	data_base[this->status_code].generate_error(*this, req.config_block.__DefaultErrorpg);
}

void response::send_response(data &req)
{
	std::cout << header_resp << std::endl;
	if(req.location.__Redirection[0].length() != 0)
	{
		write(req.client_socket, header_resp.c_str(), strlen(header_resp.c_str()));

		return ;
	}
	else if (req.root_cgi.length() == 0)
	{
		write(req.client_socket, header_resp.c_str(), strlen(header_resp.c_str()));
	}
	if (this->lenth > 0)
	{
		this->fd = open(req.path.c_str(), O_RDONLY);
		char *buff = new char[this->lenth];
		read(this->fd, buff, this->lenth);
		write(req.client_socket, buff, this->lenth);
		close(this->fd);
		delete[] buff;
	}
}