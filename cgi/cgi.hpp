#pragma once
#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "data.hpp"

extern char **environ;
class cgi
{

public:
    cgi();
    ~cgi();
    void executecgi(data req);
};

