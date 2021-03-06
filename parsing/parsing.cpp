/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboudhir <hboudhir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 22:27:26 by hboudhir          #+#    #+#             */
/*   Updated: 2022/05/19 09:46:22 by hboudhir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "parsing.hpp"
#include "../utils/Colors.hpp"
#include <cstdlib>
#include <map>
#define LOG(X)	std::cout << X << std::endl

void	ft_split(std::string const& src, const char delimiter, std::vector<std::string> &dst)
{
	std::istringstream iss(src);
	
	do
	{
		std::string subs;
		iss >> subs;
		if (subs.size() > 0)
		dst.push_back(subs);
	} while (iss);
}

void	exitMessage(int exitStatus, std::string message, int ln=0)
{
	std::cerr << RED << message;
	if (ln)
		std::cerr << ln;
	std::cerr << std::endl << RESET;
	exit(exitStatus);
	// return 00;
}


void	checkMandatory(ServerBlock& config, int serverLN)
{
	if (config.__Port <= 0)
		exitMessage(1, "Error: Missing directive (Port number) in server: ", serverLN);
	if (!config.__Host.size())
		exitMessage(1, "Error: Missing directive (Host) in server: ", serverLN);
}

//! Currently this function manages the port and limitSize directives.
//! (whether the bool dir variable is set to true or false)
void	checkPort(std::vector<std::string> &line, ServerBlock &config, int ln, bool dir=false)
{
	int nb = 0;
	
	if (line.size() != 2)
		exitMessage(1, "Error! Wrong number of arguments in line: ", ln);

	for (int i = 0; i < line[1].size(); i++)
		if (!(std::isdigit(line[1][i])))
			exitMessage(1, "Error! positive numeric argument only, line: ", ln);
	std::istringstream(line[1]) >> nb;
	if (nb <= 0)
		exitMessage(1, "Error: wrong argument value: ", ln);
	if (dir == true)
		std::istringstream(line[1]) >> config.__Port; // Check if the value is absolutely positive.
	else
		std::istringstream(line[1]) >> config.__ClientLimit;
}

/* 
	Should I check the given host? or take it as it is.
	//? I think host should be an IP address, meaning digits only seperated by dots.
*/
void	checkHost(std::vector<std::string> &line, ServerBlock &config, int ln)
{
	if (line.size() != 2)
		exitMessage(1, "Error! Wrong number of arguments in line: ", ln);
	config.__Host = line[1];
}

//! Currently this function manages the errorPages and serverNames directives.
//! (whether the bool dir variable is set to true or false)
void	checkServerNames(std::vector<std::string> &line, ServerBlock &config, int ln, bool dir=false)
{
	std::vector<std::string>::iterator it;
	if (line.size() < 2)
		exitMessage(1, "Error! too few arguments in line: ", ln);
	if (dir == false && line.size() % 2 == 0) //* The errorPages directive needs to be pair (status code + <URL>)
		exitMessage(1, "Error! Incomplete arguments in line: ", ln);
	it = line.begin();
	it++;
	for (; it != line.end(); ++it)
		if(dir == true)
			config.__ServerNames.push_back(*it);
		else
			config.__DefaultErrorpg.push_back(*it);
	//TODO: In the case of errorPages (dir == false) check each pair(status code)
	//TODO: whether it contains digits only or not.
}


void	allowedMethods(std::vector<std::string>& line, Locations& location, int ln)
{
	std::vector<std::string>::iterator it;
	
	if (line.size() < 2)
		exitMessage(1, "Error! too few arguments in line: ", ln);
	it = line.begin();
	it++;
	for (; it != line.end(); ++it) // Check for GET POST AND DELETE ONLY
		location.__AllowedMethods.push_back(*it);
}

//TODO: Check whether the vector content is pair or not
/* Vector content: [301][redirectionURL] */
/* 	  impair element ^ || pair element &*/
void	redirections(std::vector<std::string>& line, Locations& location, int ln)
{
	std::vector<std::string>::iterator it;
	
	if (line.size() < 2)
		exitMessage(1, "Error! too few arguments in line: ", ln);
	it = line.begin();
	it++;
	for (; it != line.end(); ++it)
		location.__Redirection.push_back(*it);
}

void	root(std::vector<std::string> &line, Locations &location, int ln)
{
	if (line.size() != 2)
		exitMessage(1, "Error! Wrong number of arguments in line: ", ln);
	location.__Root = line[1];
}

void	directoryListing(std::vector<std::string>& line, Locations &location ,int ln)
{
	if (line.size() != 2)
		exitMessage(1, "Error! Wrong number of arguments in line: ", ln);
	if (line[1] == "on")
		location.__DirList = true;
	else if (line[1] == "off")
		location.__DirList = false;
	else
		exitMessage(1, "Error! Wrong argument in line: ", ln);
}

void	defaultFile(std::vector<std::string>& line, Locations& location, int ln)
{
	if (line.size() != 2)
		exitMessage(1, "Error! Wrong number of arguments in line: ", ln);
	location.__DefaultFile = line[1];
}

void	route(std::vector<std::string>& line, Locations& location, int ln)
{
	if (line.size() != 2)
		exitMessage(1, "Error! Wrong number of arguments in line: ", ln);
	location.__DefaultFile = line[1];
}
/*============================================================================*/
//TODO: I should probably make a generic function that fills a vector.
		// else if (tmp.size() > 0 && tmp[0] == "\"redirections\":")
		// 	fillVector(tmp, NULL, location, ln);
/*============================================================================*/

void	log_data(ServerBlock& config)
{
	std::vector<std::string>::iterator	it;
	LOG("================= Data log =================");
	// LOG("Port 		=====> " << config.__Port);
	// LOG("Host 		=====> " << config.__Host);
	// LOG("ServerNames 	=====>");
	// it = config.__ServerNames.begin();
	// for(; it != config.__ServerNames.end(); it++)
	// 	LOG("			" << *it);
	// LOG("ErrorPages 	=====>");
	// it = config.__DefaultErrorpg.begin();
	// for(; it != config.__DefaultErrorpg.end(); it++)
	// 	LOG("			" << *it);
	// LOG("LimitSize	=====> " << config.__ClientLimit);
	// LOG("Location: ");
	// LOG("	methods: ====>");
	// it = config.__Locations[0].__AllowedMethods.begin();
	// for (; it !=config.__Locations[0].__AllowedMethods.end(); it++)
	// 	LOG("			" << *it);
	LOG("Port: " << config.__Port);
	LOG("Host: " << config.__Host);
	std::cout << "ServerNames: ";
	it = config.__ServerNames.begin();
	for(; it != config.__ServerNames.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	std::cout << "ErrorPages: ";
	it = config.__DefaultErrorpg.begin();
	for(; it != config.__DefaultErrorpg.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	LOG("LimitSize: " << config.__ClientLimit);
	std::cout << "============================\nLocation:" << std::endl;
	std::cout << "Route: " << config.__Locations[0].__Route << std::endl;
	std::cout << "AllowedMethods: ";
	it = config.__Locations[0].__AllowedMethods.begin();
	for(; it != config.__Locations[0].__AllowedMethods.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	std::cout << "Redirections: ";
	it = config.__Locations[0].__Redirection.begin();
	for(; it != config.__Locations[0].__Redirection.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	LOG("Root: " << config.__Locations[0].__Root);
	LOG("DirList: " << std::boolalpha << config.__Locations[0].__DirList);
	LOG("DefaultFile : " << config.__Locations[0].__DefaultFile);












	
	LOG("============================");
	
	LOG("============================================");

}

void	locationBlock(std::ifstream &ifs, std::string line, int &ln, ServerBlock& config)
{
	int start = 				int(ln);
	std::vector<std::string>	tmp;
	Locations					location;

	ft_split(line, ' ', tmp);
	if (tmp.size() != 2 && tmp[1] != "{")
		exitMessage(1, "missing '{' in line: ", ln);
	else if (tmp.size() > 2)
		exitMessage(1, "unknown argument in line: ", ln);

	while (tmp[0] != "}") // Location block loop.
	{
		tmp.clear();
		ln++;
		if (!std::getline(ifs, line)) // Reached the end of the file.
			exitMessage(1, "missing '}' after line: ", start);
		else if (line[0] == '#')
			continue ;

		ft_split(line, ' ', tmp);
		if (tmp.size() > 0 && tmp[0] == "\"route\":")
			route(tmp, location, ln);
		if (tmp.size() > 0 && tmp[0] == "\"methods\":")
			allowedMethods(tmp, location, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"redirection\":") // there's only two arguments for redirections.
			redirections(tmp, location, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"root\":")
			root(tmp, location, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"autoindex\":") // AKA directory listing.
			directoryListing(tmp, location, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"Default\":")
			defaultFile(tmp, location, ln);
		else if (tmp[0] != "}" && tmp.size() > 0)
			exitMessage(1, "unknown directive line: ", ln);
		else if (tmp.size() == 1 && tmp[0] == "}")
			break ;
		// else if (tmp.size() > 0 && (tmp[0] != "}" && tmp.size() != 1))
	}
	if (tmp.size() > 1)
		exitMessage(1, "unknown directive line: ", ln);
	config.__Locations.push_back(location);
}

//TODO:
/* 
	* Check whether the given server block is the first one or not. (For the default server boolean)
*/
void	serverBlock(std::ifstream &ifs, int &ln, ConfigFile &config)
{
	std::string line;
	int		start = 			int(ln); // to throw the line of the error in case '}' not found.
	std::vector<std::string>	tmp;
	ServerBlock					server;

	while(line[0] != '}') // Server block loop
	{
		tmp.clear();
		ln++;
		if (!std::getline(ifs, line))
			exitMessage(1, "missing '}' after line: ", start);
		else if (line[0] == '#')
			continue ;
		//* Inside server block parsing starts here.
		//TODO: check what command.
		//TODO: split the given string by spaces.
		ft_split(line, ' ', tmp);
		if (tmp.size() > 0 && tmp[0] == "\"port\":")
			checkPort(tmp, server, ln, true);
		else if (tmp.size() > 0 && tmp[0] == "\"limitSize\":")
			checkPort(tmp, server, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"host\":")
			checkHost(tmp, server, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"serverNames\":")
			checkServerNames(tmp, server, ln, true);
		else if (tmp.size() > 0 && tmp[0] == "\"errorPages\":")
			checkServerNames(tmp, server, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"location\":") //todo: doesn't check whether there is an opening bracket or not
			locationBlock(ifs, line, ln, server);
		// else if (tmp.size() > 0 && (tmp[0] != "}" && tmp.size() != 1))
		else if (tmp[0] != "}" && tmp.size() > 0)
			exitMessage(1, "unknown directive line: ", ln);
		// LOG("value: " << std::boolalpha << (tmp[0] != "}" && tmp.size() != 1) << " in line: " << ln);
	}
	if (line.size() > 1)
		exitMessage(1,"unknown directive after '}' in line: ", ln);
	checkMandatory(server, start); //* Probably should be in serverBlock(). (to check each server object one at the time)
	log_data(server);
	config.__Servers.push_back(server);
}




//TODO: list of things to check later.
/* 
	* Check duplicate arguments.
	* Add another struct that contains a vector of ServerBlock's.
	* Create a temporary variable to append to the ServerBlock's vector.
*/
void	parse(char *file, ConfigFile &config)
{
	int							__LN; // Line number. (of the config file)
	std::ifstream				ifs(file);
	std::string					line;
	std::vector<std::string>	test; // Splitting vector.
	ServerBlock					server;
	
	// Checking whether the given configuration file is valid or not. (permissions, exists ...)
	if(!ifs.good())
		exitMessage(1, "Something is wrong with the configuration file.\nPlease check if the given file is valid: if it exists and it got reading permission.");

	__LN = 0;
	// The parsing file loop.
	while(std::getline(ifs, line) && ++__LN)
	{
		if (line[0] == '#') // found a comment, skipped.
			continue ;
	
		if (line == "server {") // Beginning of the server block.
			serverBlock(ifs, __LN, config);
		else if (!line.empty())
			exitMessage(1, "unknown directive in line: ", __LN);
	}
	// checkMandatory(server); //* Probably should be in serverBlock(). (to check each server object one at the time)
	
	// Debugging function.
	// log_data(server);
	// config.__Servers.push_back(server);
	ifs.close();
}