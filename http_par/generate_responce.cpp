// #include <iostream>
#include  "../webserve_data.hpp"
#include "../parsing/parsing.hpp"


/* 

class ConfigFile
{
	public: //* To be changed to private if desired later.
		std::vector<ServerBlock>		__Servers;
};

 */

int check_first_line(data &d) // parsing the first line.
{
	// check method

	// first check if LOWERCASE
	const char *save = d.method.c_str();
	for(size_t i = 0; save[i] != '\0'; i++)
	{
		if(islower(save[i]))
		{
			std::cout << "bad" << std::endl;
			d.status_code = 400;
			break;
		}
	}

	// check if file exist or have permision
	
	return 0;

}


int main()
{
	data d;

	d.method = "GeT";
	check_first_line(d);
	std::cout << "code = > " << d.status_code << std::endl;

}