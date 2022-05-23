// #include "../parsing/parsing.hpp"
#include "../webserv.hpp"
// void check_first_line(data &d, ConfigFile &conf) // parsing the first line.
// {
//  	const char *save = d.method.c_str();
// 	for(size_t i = 0; save[i] != '\0'; i++)
// 	{
// 		if(islower(save[i]))
// 		{
// 			// std::cout << "bad" << std::endl;
// 			d.status_code = 400;
// 			throw 400;
// 		}
// 	}
// 	// GET POST DELETE
// 	if(d.method != "GET" || d.method != "POST" || d.method != "DELETE")
// 	{
// 		d.status_code = 405;
// 		throw 405;
// 	}
// }

//! Should have a SeverBlock parametre instead of locations.
//TODO: iterate through the locations vector instead of loop.
void check_url_path(data &req, Locations *conf) // check url ==> GET /index.html http
{
	// 1 - location block == > /test/test/test/index.html
	// locations /test/test
	// 2 - file exists :(
	// 3 - file's permissions
	// 4 - file's extension
	bool is= false;
	std::istringstream to_split(req.path);
	std::string tmp;
	std::string finalString;
	while (getline(to_split, tmp, '/'))
	{
		if (tmp.size() > 0)
		{
			// /test1/test2/test3
			// the loop
			//if found overide
			//else continue
			std::cout << "line --> " << tmp << std::endl;
			finalString.append(tmp + "/");
		}
	}
	if(finalString.length() > 0)	
		finalString.erase(finalString.length() - 1, 1);
	req.path.clear();
	req.path = finalString;

	// std::vector<Locations>::iterator it = __Locations.begin();
	
	/* 
		example.com/test1/test2/index.html

		location /test1 { || location /test1/test2/test3{
	 */
	for (int i = 0; i < 4; i++)
	{		
		if(req.path == 	conf[i].__Route ||	(req.path.length() == 0 && conf[i].__Route == "/"))
		{
			std::cout << "yes" << std::endl;
			req.location = conf[i];
			is = true;
			break;
		}
		else
			std::cout << "No" << std::endl;	
	}
	if (is ==  false)
		std::cout << "is false" << std::endl;
	
	// route /test1/test2/test3
	// index.html
	/*

	 */ 
	// for(; it != conf.end(); it++)
	// {
	// 	if(req.path == (*it).__Route)
	// 	{
	// 		std::cout << "yes" << std::endl;
	// 	}
	// 	else
	// 		std::cout << "No" << std::endl;
	// }

	std::cout << "The final string: " << finalString << std::endl;
}



int main()
{
	
	std::string  test = "";
	Locations	temp[4];
	temp[0].__Route =  "test1";
	temp[1].__Route =  "test1/test2";
	
	temp[2].__Route =  "test1/test2/test3";
	temp[3].__Route =  "/";
	data da;
	da.path = "/";
	check_url_path(da, temp);
	// /test/test/test/test/
}