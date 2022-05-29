#include "../webserv.hpp"

int is_dir_and_exist(const char *path)
{
    struct stat path_stat;
    int exist = stat(path, &path_stat);
	if(exist == 0)
		std::cout << "found" << std::endl;
	else
		std::cout << "not found" << std::endl;
    return S_ISDIR(path_stat.st_mode);
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
	
	// 	replace route to root
	if(req.location.__Root.length() == 0)
		req.path.replace(0, req.location.__Route.length() - 1, "./default");
	else
		req.path.replace(0, req.location.__Route.length() - 1, req.location.__Root);
	std::cout << "last info" << std::endl;
	std::cout << "final path ===> " << req.path << std::endl;
	std::cout << "location route ===> " << req.location.__Route << std::endl;
	if(is_dir_and_exist(req.path.c_str()) != 0)
	{
		std::cout << "is dir" << std::endl;
		if(req.location.__DefaultFile.length() != 0)
		{
			// default file

		}
		else if (req.location.__DirList  == true)
		{
			// autoindex

		}
		else
			throw 403;
		//1. default file
		//2. autoindex
		//3. throw error
	}
	else
	{
		int fd  = open(req.path.c_str(), O_RDONLY);
		if(fd < 0)
			std::cout << "you dont have permission" << std::endl;
		std::cout << "is safe file you can read from it" << std::endl;
		close(fd);
	}
	/* 
		url: ./default/T3

		location:route T1/T2 
	
	 */
	// std::cout << "The final string: " << finalString << std::endl;
	// is dir

}



// int main()
// {
// 	std::string  test = "";
// 	Locations	temp[4];
// 	temp[0].__Route =  "/test1";
// 	temp[1].__Route =  "/test1/test2";
// 	// temp[1].__Root = "/tmp";
	
// 	temp[2].__Route =  "/test1/test2/test3";
// 	temp[3].__Route =  "/";
// 	data da;
// 	da.path = "/test1/test2/test4/index.html";

// 	check_url_path(da, temp);
// 	// /test/test/test/test/
// }