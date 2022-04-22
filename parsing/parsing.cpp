/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboudhir <hboudhir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 22:27:26 by hboudhir          #+#    #+#             */
/*   Updated: 2022/04/22 03:39:00 by hboudhir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "parsing.hpp"
#include "../utils/Colors.hpp"


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
}

//! Currently this function manages the port and limitSize directives.
//! (whether the bool dir variable is set to true or false)
void	checkPort(std::vector<std::string> &line, ServerBlock &config, int ln, bool dir=false)
{
	if (line.size() != 2)
		exitMessage(1, "Error! Wrong number of arguments in line: ", ln);

	for (int i = 0; i < line[1].size() - 1; i++)
		if (!(std::isdigit(line[1][i])))
			exitMessage(1, "Error! numeric argument only, line: ", ln);
	if (dir == true)
		std::istringstream(line[1]) >> config.__Port;
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
	it = line.begin();
	it++;
	for (; it != line.end(); ++it)
		if(dir == true)
			config.__ServerNames.push_back(*it);
		else
			config.__DefaultErrorpg.push_back(*it);
	//TODO: In the case of errorPages (dir == false) check each pair(status code)
	//TODO: wether it contains digits only or not.
}

void	locationBlock(std::ifstream &ifs, std::string line, int &ln, ServerBlock& config)
{
	int start = int(ln);
	std::vector<std::string> tmp;

	ft_split(line, ' ', tmp);
	if (tmp.size() != 2 && tmp[1] != "{")
		exitMessage(1, "missing '{' in line: ", ln);
	while (tmp[0] != "}") // Location block loop,
	{
		tmp.clear();
		ln++;
		LOG(ln << ": " << line);
		if (!std::getline(ifs, line))
			exitMessage(1, "missing '}' after line: ", start);
		ft_split(line, ' ', tmp);
		if (tmp.size() > 0 && (tmp[0] != "}" && tmp.size() != 1))
			exitMessage(1, "unknown directive line: ", ln);
		else if (tmp.size() == 1 && tmp[0] == "}")
			break ;
	}
	if (tmp.size() > 1)
		exitMessage(1, "unknown directive line: ", ln);
}

//TODO:
/* 
	* Check whether the given server block is the first one or not. (For the default server boolean)
*/
void	serverBlock(std::ifstream &ifs, int &ln, ServerBlock &config)
{
	std::string line;
	int		start = int(ln); // to throw the line of the error in case '}' not found.
	std::vector<std::string> tmp;
	
	while(line[0] != '}') // Server block loop
	{
		tmp.clear();
		ln++;
		if (!std::getline(ifs, line))
			exitMessage(1, "missing '}' after line: ", start);
		//* Inside server block parsing starts here.
		//TODO: check what command.
		//TODO: split the given string by spaces.
		ft_split(line, ' ', tmp);
		if (tmp.size() > 0 && tmp[0] == "\"port\":")
			checkPort(tmp, config, ln, true);
		else if (tmp.size() > 0 && tmp[0] == "\"limitSize\":")
			checkPort(tmp, config, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"host\":")
			checkHost(tmp, config, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"serverNames\":")
			checkServerNames(tmp, config, ln, true);
		else if (tmp.size() > 0 && tmp[0] == "\"errorPages\":")
			checkServerNames(tmp, config, ln);
		else if (tmp.size() > 0 && tmp[0] == "\"location\":") //todo: doesn't check whether there is an opening bracket or not
			locationBlock(ifs, line, ln, config);
		else if (tmp.size() > 0 && (tmp[0] != "}" && tmp.size() != 1))
			exitMessage(1, "unknown directive line: ", ln);
	}
	if (line.size() > 1)
		exitMessage(1,"unknown directive after '}' in line: ", ln);
}

void	log_data(ServerBlock& config)
{
	std::vector<std::string>::iterator	it;
	LOG("================= Data log =================");
	LOG("Port 		=====> " << config.__Port);
	LOG("Host 		=====> " << config.__Host);
	LOG("ServerNames 	=====>");
	it = config.__ServerNames.begin();
	for(; it != config.__ServerNames.end(); it++)
		LOG("			" << *it);
	LOG("ErrorPages 	=====>");
	it = config.__DefaultErrorpg.begin();
	for(; it != config.__DefaultErrorpg.end(); it++)
		LOG("			" << *it);
	LOG("LimitSize	=====> " << config.__ClientLimit);
		
	LOG("============================================");

}

//TODO: list of things to check later.
/* 
	* Check duplicate arguments.
*/
void	parse(char *file, ServerBlock &config)
{
	int							__LN; // Line number. (of the config file)
	std::ifstream				ifs(file);
	std::string					line;
	std::vector<std::string>	test; // Splitting vector.

	// Checking whether the given configuration file is valid or not. (permissions, exists ...)
	if(!ifs.good())
	{
		std::cerr << RED;
		std::cerr << "Something is wrong with the configuration file." << std::endl;
		std::cerr << "Please check if the given file is valid: if it exists and it got reading permission." << std::endl;
		std::cerr << RESET;
		exit(1);
	}

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
	// Debugging function.
	log_data(config);
	ifs.close();
}