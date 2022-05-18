
#include "cgi.hpp"

cgi::cgi()
{
}

cgi::~cgi()
{
}
void cgi::executecgi(data req)
{

    setenv("GATEWAY_INTERFACE", "CGI/1.1",1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1",1);
    setenv("REDIRECT_STATUS", req.status_code,1);
    setenv("SCRIPT_FILENAME", req.path,1);
    setenv("REQUEST_METHOD", req.method,1);
    setenv("CONTENT_LENGTH", req.content_length,1);
    setenv("QUERY_STRING", req.query_string,1);
    setenv("CONTENT_TYPE", req.content_type,1);

    const char*  args[3] = {
        "/usr/bin/php-cgi",
        // "test.php", // not important 
        NULL
    };

    int input = open(req.body_path, O_RDONLY);                    // path of the reques file
    int output = open(req.response_path, O_CREAT | O_WRONLY | O_TRUNC, 0777);     // path of where you ant to save the body response

    int pid = fork(); 
    if (pid == 0)
    {
        dup2(input, 0); 
        dup2(output, 1);

        execve("/usr/bin/php-cgi", (char**)args, environ);
    }
    else
    {
        waitpid(pid, NULL, 0);
        close(input);
        close(output);
    }
}
