#pragma once

#include <cstdint>
#include <ostream>
#include <sys/socket.h> // sokcet header :p
#include <sys/select.h> //  select header
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <unistd.h> // for close 
#include <fstream>  // header for create file 
#include <netinet/in.h> // structer sockaddr_in header
#include <arpa/inet.h> // header for inet_addr
#include <fcntl.h> // fcntl hreader 
#include <stdio.h> // for remove() function
#include "../parsing/parsing.hpp"
#include <map>
void install_servers(ConfigFile &conf);
