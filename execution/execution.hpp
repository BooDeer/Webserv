#pragma once

#include <cstdint>
#include <ostream>
#include <sys/socket.h>
#include <sys/select.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include "../parsing/parsing.hpp"
#include <map>

void install_servers(ConfigFile &conf);
