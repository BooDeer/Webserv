#include "webserv.hpp"

void data::create_file(int fd_socket, int client_socket)
{
    this->server_socket = fd_socket;
    this->client_socket = client_socket;
	std::cout << "start create file-----" << std::endl;
	struct timeval	time;
	long long t;
    gettimeofday(&time, NULL);
    t = (time.tv_usec / 1000) + (time.tv_sec * 1000);
    std::stringstream save, save2, time222;
    save << client_socket;
    std::string id_c;
    save >> id_c;
    save2 << server_socket;
    std::string id_s;
    save2 >> id_s;
    time222 << t;
    std::string time_;
    time222 >> time_;
    _fileName = std::string("/tmp/webServ_" + id_c + "_" + id_s + "_" + time_ + ".txt");
    std::cout << "writing in : " << _fileName << std::endl;
    std::cout << " file create fot this request is " <<  _fileName << std::endl;
}

int main(int ac, char **av)
{
	ConfigFile		config; // <=== All the parsing data. (AKA the server blocks)
	if (ac != 2) //TODO:Should use the default config file if no config file has been given.
	{
		std::cerr << "usage: ./webserv " << CYAN << "[Configuration file]" << std::endl;
		return (1);
	}

	parse(av[1], config); // parse data  from config file
// here ?
	install_servers(config); // include config file setting
	// system("leaks webserv");
}

