/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboudhir <hboudhir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 22:27:26 by hboudhir          #+#    #+#             */
/*   Updated: 2022/04/18 06:08:57 by hboudhir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"
#include "../utils/Colors.hpp"

void	serverBlock(std::ifstream &ifs, int &ln)
{
	std::string line;
	int		start = int(ln);

	while(line[0] != '}') // Server block loop
	{
		ln++;
		if (!std::getline(ifs, line))
		{
			std::cerr << RED << "missing '}' after line: " << start << std::endl << RESET;
			exit (1);
		}
		//* Inside server block parsing starts here.
		std::cout << line << std::endl;
		std::stringstream X(line);
	}
	if (line.size() > 1)
	{
		std::cerr << RED << "unknown command after '}' in line: " << ln << std::endl << RESET;
		exit(1);
	}
	std::cout << "finished block section" << std::endl;
}

void	parse(char *file)
{
	int							__LN; // Line number. (of the config file)
	std::ifstream				ifs(file);
	std::string					line;
	std::vector<std::string>	test;

	if(!ifs.good())
	{
		std::cerr << RED;
		std::cerr << "Something is wrong with the configuration file." << std::endl;
		std::cerr << "Please check if the given file is valid: if it exists and it got reading permission." << std::endl;
		std::cerr << RESET;
		exit(1);
	}
	__LN = 0;
	while(std::getline(ifs, line) && ++__LN)
	{
		if (line[0] == '#') // found a comment.
			continue ;
		
		if (line == "server {") // Beginning of the server block.
			serverBlock(ifs, __LN);
		else if (!line.empty())
		{
			std::cerr << RED << "unknown command in line: " << __LN << std::endl << RESET;
			exit(1);
		}
	}
	ifs.close();
}