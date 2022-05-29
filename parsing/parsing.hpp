#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include "../utils/Colors.hpp"
#include "configFile/Config.hpp"

void	parse(char *file, ConfigFile& config);
void	exitMessage(int exitStatus, std::string message, int ln = 0);