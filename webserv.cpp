#include "webserv.hpp"

void data::create_file(int fd_socket, int client_socket)
{
	struct timeval	time;
	long long t;
    std::stringstream save, save2, time222;
    std::string id_c, id_s, time_;

    this->server_socket = fd_socket;
    this->client_socket = client_socket;
    gettimeofday(&time, NULL);
    t = (time.tv_usec / 1000) + (time.tv_sec * 1000);
    save << client_socket;
    save >> id_c;
    save2 << server_socket;
    save2 >> id_s;
    time222 << t;
    time222 >> time_;
    _fileName = std::string("/tmp/webServ_" + id_c + "_" + id_s + "_" + time_ + ".txt");
}

int main(int ac, char **av)
{
	ConfigFile		config;
	if (ac != 2)
	{
		std::cerr << "usage: ./webserv " << CYAN << "[Configuration file]" << std::endl;
		return (1);
	}

	parse(av[1], config);
	install_servers(config);
}

