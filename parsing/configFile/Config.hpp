	#pragma once

#include <iostream>
#include <vector>
#include <map>

class Locations
{
	public:
		std::string					__Route; // location/extension.
		std::string					__RouteCGI; // location/extension.
		std::vector<std::string>	__AllowedMethods;
		std::string					__Redirection[2];
		std::string					__Root;
		std::string					__RootCGI;
		bool						__DirList;
		std::string					__DefaultFile;
		//? ◦ Execute CGI based on certain file extension (for example .php).
		Locations()
		{
			this->__Route = "./default/";
		} // permissiosn a zebi pls :D
	
};

class ServerBlock //! Name to be changed later
{
	/* 
	int i = socket(); <=== of the server
	 */
	//? I should probably initialize this class.
	public:
		unsigned short							__Port; // I choose unsigned short because it can hold exactly up to 65535.
		std::string								__Host; // Host name of the server.
		//? What's the difference between the server_names and the host.
		//* I guess server_names mean aliases to that host. (similar to DNS)
		std::vector<std::string>				__ServerNames;
		//? Should this be a vector of strings or (?) . . .
		std::map<std::string, std::string>				__DefaultErrorpg;
		unsigned long long 						__ClientLimit; // Limit client body size.
		bool									__DefaultServer; // The default server. (Mainly the first server)
		std::vector<Locations>					__Locations;	//
	public:
		//TODO: Take functions out of header file.
		ServerBlock( void ): __Port(0), __ClientLimit(0)
		{

		};
		// ~ServerBlock( void );
};

class ConfigFile
{
	public: //* To be changed to private if desired later.
		std::vector<ServerBlock>		__Servers;
		int								serverFd; // <==== ?
	// ConfigFile &operator==(const ConfigFile &copy)
	// {
	// 	this->__Servers = 
	// 	return *this;
	// }
};