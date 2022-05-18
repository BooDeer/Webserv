#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "cgi.hpp"
int main ()
{
    data req;
    cgi c;
    c.executecgi(req);
}