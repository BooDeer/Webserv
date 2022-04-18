#pragma once

#include <iostream>
#include <vector>

class Locations
{
	private:

		std::vector<std::string>	__AllowedMethods;
		std::string					__Redirection;
		std::string					__SearchDir;
		bool						__DirList;
		std::string					__DefaultFile;
		//? ◦ Execute CGI based on certain file extension (for example .php).
	
};

class ServerBlock //! Name to be changed later
{

	private:
		unsigned short			__Port; // I choose unsigned short because it can hold exactly up to 65535.
		std::string				__Host; // Host name of the server.
		//? What's the difference between the server_names and the host.
		//* I guess server_names mean aliases to that host. (similar to DNS)
		std::string				__ServerNames;
		//? Should this be a vector of strings or (?) . . .
		std::string				__DefaultErrorpg;
		unsigned long long 		__ClientLimit; // Limit client body size.
		bool					__DefaultServer; // The default server. (Mainly the first server)
		std::vector<Locations>	__Locations;	//
	public:
		// ServerBlock( void );
		// ~ServerBlock( void );
};