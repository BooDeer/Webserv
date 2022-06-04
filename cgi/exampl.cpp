#include "../webserv.hpp"


char **environ;

int main()
{
    

    const char*  args[3] = {
        "/usr/bin/php-cgi",
        "test.php", // not important 
        NULL
    };

    // int input = open("body-msg", O_RDONLY);                    // path of the reques file
    int output = open("response.txt", O_CREAT | O_WRONLY | O_TRUNC, 0777);     // path of where you ant to save the body response

    int pid = fork(); // 
    if (pid == 0)
    {
        setenv("GATEWAY_INTERFACE", "CGI/1.1",1);
        setenv("SERVER_PROTOCOL", "HTTP/1.1",1);
        setenv("REDIRECT_STATUS", "200",1);
        setenv("SCRIPT_FILENAME", "test.php",1);
        setenv("REQUEST_METHOD", "GET",1);
        // setenv("CONTENT_LENGTH", "1",1);
        // setenv("QUERY_STRING", "user=Admin&pass=lcodepoincom1234",1);
        // setenv("CONTENT_TYPE", "multipart/form-data; boundary=----WebKitFormBoundaryrEvtmhEJEIqn43cE",1);
        setenv("CONTENT_TYPE", "application/x-httpd-php", 1);

        // dup2(input, 0); 
        dup2(output, 1);
        execve("/goinfre/ssamadi/.brew/bin/php-cgi", (char**)args, environ);

    }
    else
    {
        waitpid(pid, NULL, 0);
      //  close(input);
        close(output);
    }
}


