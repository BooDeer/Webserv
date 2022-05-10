#include "webserv.hpp"

int main(int ac, char **av)
{
	// ServerBlock	config;
	ConfigFile		config; // <=== All the parsing data. (AKA the server blocks)
	if (ac != 2) ///TODO:Should use the default config file if no config file has been given.
	{
		std::cerr << "usage: ./webserv " << CYAN << "[Configuration file]" << std::endl;
		return (1);
	}

	parse(av[1], config); // parse data  from config file 
	// std::cout << config.__Servers.size() << std::endl;





	// webServ entery oint?
	install_servers(config); // innlude config file setting
	// std::cout << config.__Servers[0].__Host << std::endl;
	// system("leaks webserv");
}