#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "parsing/parsing.hpp"
#include "utils/Colors.hpp"
#include "webserve_data.hpp"




#include "webserv.hpp"
int main(int ac, char **av)
{
	// ServerBlock	config;
	ConfigFile		config; // <=== All the parsing data. (AKA the server blocks)
	if (ac != 2) ///! <==== Should use the default config file if no config file has been given.
	{
		std::cerr << "usage: ./webserv " << CYAN << "[Configuration file]" << std::endl;
		return (1);
	}

	parse(av[1], config); // parse data  from config file 

	// webServ entery point?
	
	// std::cout << config.__Servers[0].__Host << std::endl;
	// system("leaks webserv");
}