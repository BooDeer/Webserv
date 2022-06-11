/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboudhir <hboudhir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 22:27:26 by hboudhir          #+#    #+#             */
/*   Updated: 2022/06/11 04:52:15 by hboudhir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "parsing.hpp"
#include <cstdlib>
#include <map>
#include <cctype>

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

void	exitMessage(int exitStatus, std::string message, int ln)
{
	std::cerr << RED << message;
	if (ln)
		std::cerr << ln;
	std::cerr << std::endl << RESET;
	exit(exitStatus);
}


void	checkMandatory(ServerBlock& config, int serverLN)
{
	if (config.__Port <= 0)
		exitMessage(1, "Error: Missing directive (Port number) in server: ", serverLN);
	if (!config.__Host.size())
		exitMessage(1, "Error: Missing directive (Host) in server: ", serverLN);
}

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
		std::istringstream(line[1]) >> config.__Port;
	else
		std::istringstream(line[1]) >> config.__ClientLimit;
}

void	checkHost(std::vector<std::string> &line, ServerBlock &config, int ln)
{
	if (line.size() != 2)
		exitMessage(1, "Error! Wrong number of arguments in line: ", ln);
	config.__Host = line[1];
}

void	checkServerNames(std::vector<std::string> &line, ServerBlock &config, int ln, bool dir=false)
{
	std::vector<std::string>::iterator it;
	int i = 1;
	if (line.size() < 2)
		exitMessage(1, "Error! too few arguments in line: ", ln);
	it = line.begin();
	it++;
	for (; it != line.end(); ++it)
	{
		if(dir == true)
		{
			if (line.size() > 2)
				exitMessage(1, "Error! Too many arguments in line: ", ln);
			config.__ServerNames.push_back(*it);
		}
		else
		{
			if (!(line.size() % 2))
				exitMessage(1, "Error! Wrong number of arguments in line: ", ln);
			if (i % 2)
			{
				if ((*it).length() != 3 || (!std::isdigit((*it)[0]) || !std::isdigit((*it)[1]) || !std::isdigit((*it)[2])))
					exitMessage(1, "Error! Arguments should be between 100-999 line: ", ln);
				config.__DefaultErrorpg[*it] = *(it + 1);
			}
			i++;
		}
	}
}


void	allowedMethods(std::vector<std::string>& line, Locations& location, int ln)
{
	std::vector<std::string>::iterator it;
	
	if (line.size() < 2)
		exitMessage(1, "Error! too few arguments in line: ", ln);
	it = line.begin();
	it++;
	for (; it != line.end(); ++it)
	{
		if (*it != "POST" && *it != "DELETE" && *it != "GET")
			exitMessage(1, "Error! Unknown HTTP method in line: ", ln);
		location.__AllowedMethods.push_back(*it);
	}
}

/* Vector content: [301][redirectionURL] */
/* 	  impair element ^ || pair element &*/
void	redirections(std::vector<std::string>& line, Locations& location, int ln)
{
	std::vector<std::string>::iterator it;
	
	if (line.size() != 3)
		exitMessage(1, "Error! Wrong number of arguments in line: ", ln);
	it = line.begin();
	it++;
	for (int i = 0; it != line.end(); ++it)
	{
		if (!i)
			if ((*it).length() != 3 || (!std::isdigit((*it)[0]) || !std::isdigit((*it)[1]) || !std::isdigit((*it)[2])))
				exitMessage(1, "Error! Arguments should be between 100-999 line: ", ln);
		location.__Redirection[i] = *it;
		i++;
	}
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
	location.__Route = line[1];
}

void	root_cgi(std::vector<std::string>& line, Locations& location, int ln)
{
	if (line.size() != 2)
		exitMessage(1, "Error! Wrong number of arguments in line: ", ln);
	location.__RootCGI = line[1];
}
void	route_cgi(std::vector<std::string>& line, Locations& location, int ln)
{
	if (line.size() != 2)
		exitMessage(1, "Error! Wrong number of arguments in line: ", ln);
	location.__RouteCGI = line[1];
}
void	cgiUploadPath(std::vector<std::string>& line, Locations& location, int ln)
{
	if (line.size() != 2)
		exitMessage(1, "Error! Wrong number of arguments in line: ", ln);
	location.__UploadCGI = line[1];
}

void	checkLocation(Locations &location)
{
	if (location.__RouteCGI.length())
	{
		if (location.__RootCGI.length() == 0)
			exitMessage(1, "Error! cgi without executable path!");
	}
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
		exitMessage(1, "Unknown argument in line: ", ln);
	while (!tmp.size() || tmp[0] != "}")
	{
		tmp.clear();
		ln++;
		if (!std::getline(ifs, line))
			exitMessage(1, "missing '}' after line: ", start);
		else if (line[0] == '#')
		{
			ft_split(line, ' ', tmp);
			continue ;
		}
		ft_split(line, ' ', tmp);
		if (tmp.size() > 0 && tmp[0] == "\"route\":")
			route(tmp, location, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"root_cgi\":")
			root_cgi(tmp, location, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"route_cgi\":")
			route_cgi(tmp, location, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"methods\":")
			allowedMethods(tmp, location, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"redirection\":")
			redirections(tmp, location, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"root\":")
			root(tmp, location, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"autoindex\":")
			directoryListing(tmp, location, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"default\":")
			defaultFile(tmp, location, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"upload_cgi\":")
			cgiUploadPath(tmp, location, ln);
		else if ( tmp.size() > 0  && tmp[0] != "}")
			exitMessage(1, "Unknown directive line: ", ln);
		else if (tmp.size() == 1 && tmp[0] == "}")
			break ;
	}
	if (tmp.size() > 1)
		exitMessage(1, "Unknown directive line: ", ln);
	checkLocation(location);
	config.__Locations.push_back(location);
}

void	serverBlock(std::ifstream &ifs, int &ln, ConfigFile &config)
{
	std::string line;
	int		start = 			int(ln);
	std::vector<std::string>	tmp;
	ServerBlock					server;

	while(line[0] != '}')
	{
		tmp.clear();
		ln++;
		if (!std::getline(ifs, line))
			exitMessage(1, "missing '}' after line: ", start);
		else if (line[0] == '#')
			continue ;
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
		else if (tmp.size() > 0 && tmp[0] == "\"location\":")
			locationBlock(ifs, line, ln, server);
		else if (tmp.size() > 0 && tmp[0] != "}")
			exitMessage(1, "Unknown directive line: ", ln);
	}
	if (line.size() > 1)
		exitMessage(1,"Unknown directive after '}' in line: ", ln);
	checkMandatory(server, start);
	config.__Servers.push_back(server);
}


void  filter_servers(ConfigFile& config)
{
	for(int i = 0; i < config.__Servers.size(); i++)
	{
		for (int j = 0; j < config.__Servers.size(); j++)
		{
			if (j != i)
			{
				if ((config.__Servers[i].__Port == config.__Servers[j].__Port) && (config.__Servers[i].__Host == config.__Servers[j].__Host))
				{
					if (!config.__Servers[i].__ServerNames.size())
						exitMessage(1, "Error! two servers have the same host:port but without server_names."); 
					else if (config.__Servers[i].__ServerNames == config.__Servers[j].__ServerNames) // same server_name
						exitMessage(1, "Error! two servers have the same host:port but with the same server_names.");
				}

			}
		}
	}
}


void	parse(char *file, ConfigFile &config)
{
	int							__LN;
	std::ifstream				ifs(file);
	std::string					line;
	std::vector<std::string>	test;
	ServerBlock					server;
	
	if(!ifs.good())
		exitMessage(1, "Something is wrong with the configuration file.\nPlease check if the given file is valid: if it exists and it got reading permission.");

	__LN = 0;
	while(std::getline(ifs, line) && ++__LN)
	{
		if (line[0] == '#')
			continue ;
	
		if (line == "server {")
			serverBlock(ifs, __LN, config);
		else if (!line.empty())
			exitMessage(1, "Unknown directive in line: ", __LN);
	}
	filter_servers(config);
	ifs.close();
}