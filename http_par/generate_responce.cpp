#include  "../webserve_data.hpp"
#include "../parsing/parsing.hpp"


void check_first_line(data &d, ConfigFile &conf) // parsing the first line.
{
 	const char *save = d.method.c_str();
	for(size_t i = 0; save[i] != '\0'; i++)
	{
		if(islower(save[i]))
		{
			// std::cout << "bad" << std::endl;
			d.status_code = 400;
			throw 400;
		}
	}
	// GET POST DELETE
	if(d.method != "GET" || d.method != "POST" || d.method != "DELETE")
	{
		d.status_code = 405;
		throw 405;
	}
}

void check_url(data &req) // check url ==> GET /index.html http
{
	// 1 - location block == > /test/test/test/index.html
	// locations /test/test
	// 2 - file exists :(
	// 3 - file's permissions
	// 4 - file's extension
}



// (int main()
// {
// 	// data d;
// 	data d;
// 	// create_request(5, 10);
// 	std::fstream	fd("z");

// 	if (fd.is_open())
// 	{
// 		std::cout << "kayn" << std::endl;
// 	}
// 	else
// 	{
// 		std::cout << "mkaynx" << std::endl;
// 	}
// 		return 1;
// 	// d.method = "GeT";
// 	// check_first_line(d);
// 	// std::cout << "code = > " << d.status_code << std::endl;

// })