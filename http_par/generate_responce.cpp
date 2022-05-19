#include  "../webserve_data.hpp"
#include "../parsing/parsing.hpp"


/* 

class ConfigFile
{
		std::vector<ServerBlock>		__Servers;
};

 */

void check_first_line(data &d, ConfigFile &conf) // parsing the first line.
{
// 	// check method

// 	// first check if LOWERCASE
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
// void create_file_request(int client_socket, int server_socket)
// {
// 	// Had zmer fih bzf redundent work and I hate it but it's fucking OOP :D
// 	std::stringstream save, save2, time;
// 	save << client_socket;
// 	std::string id_c;
// 	save >> id_c;
// 	save2 << server_socket;
// 	std::string id_s;
// 	save2 >> id_s;
//     std::string file_name("webServ_" + id_c + "_" + id_s + ".tmp");
// 	std::fstream	fd(file_name);

// 	if (fd.is_open())
// 	{
// 		// fd.open()
// 	}
// 	else
// 	{
// 		std::cout << "mkaynx" << std::endl;
// 	}
// }


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