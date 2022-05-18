// #include <iostream>
// #include <unistd.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <fcntl.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// int main ()
// {
//     const char*  envp[] = {
//         "GATEWAY_INTERFACE=CGI/1.1",
//         "SERVER_PROTOCOL=HTTP/1.1",
//         "REDIRECT_STATUS=200",
//         "SCRIPT_FILENAME=test.php",
//         "REQUEST_METHOD=POST",
//         "CONTENT_LENGTH=239",
//         "CONTENT_TYPE=multipart/form-data;boundary=----WebKitFormBoundarylpWRPiMZMcFM46QB",
//         NULL
//     };

//     const char*  args[3] = {
//         "/usr/bin/php-cgi",
//         "test.php",
//         NULL
//     };

//     int input = open("body-msg", O_RDONLY);
//     int output = open("response.txt", O_CREAT | O_WRONLY, 666);

//     int pid = fork();
//     if (pid == 0)
//     {
//         dup2(input, 0);
//         dup2(output, 1);

//         execve("/usr/bin/php-cgi", (char**)args, (char**)envp);
//     }
//     else
//     {
//         waitpid(pid, NULL, 0);
//         close(input);
//         close(output);
//     }




// }




#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
extern char** environ;
int main (int ac, char *av[],char **aaaa)
{
//     setenv("CONTENT_LENGTH", "",1);
//     setenv("fboui", "1337",0);
//  int i = -1;
//     while (environ[++i])
//     {
//         printf("%s\n", environ[i]);
//     }
//     printf("\n\n\n\n\n\n ---------------%d----------------------\n\n\n\n\n\n\n", i);

//     return (0);

    // const char*  envp[] = {
    //     "GATEWAY_INTERFACE=CGI/1.1",
    //     "SERVER_PROTOCOL=HTTP/1.1",
    //     "REDIRECT_STATUS=200",      
    //     "SCRIPT_FILENAME=test.php", // loaction path  php file
    //     "REQUEST_METHOD=POST", // method (post or get)
    //     "CONTENT_LENGTH=226", // darori lenght (0 or remove if get method)
    //     "QUERY_STRING=user=Admin&pass=lcodepoincom1234", // in get method case had l3iba daroriya
    //     "CONTENT_TYPE=multipart/form-data; boundary=----WebKitFormBoundaryrEvtmhEJEIqn43cE", // type bayna darori
    //     NULL // mybe need more env variables; 
    // };

    setenv("GATEWAY_INTERFACE", "CGI/1.1",1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1",1);
    setenv("REDIRECT_STATUS", "200",1);
    setenv("SCRIPT_FILENAME", "test.php",1);
    setenv("REQUEST_METHOD", "POST",1);
    setenv("CONTENT_LENGTH", "226",1);
    setenv("QUERY_STRING", "user=Admin&pass=lcodepoincom1234",1);
    setenv("CONTENT_TYPE", "multipart/form-data; boundary=----WebKitFormBoundaryrEvtmhEJEIqn43cE",1);

    const char*  args[3] = {
        "/usr/bin/php-cgi",
        "test.php", // not important 
        NULL
    };

    int input = open("body-msg", O_RDONLY);                    // path of the reques file
    int output = open("response.txt", O_CREAT | O_WRONLY, 0777);     // path of where you ant to save the body response

    int pid = fork(); 
    if (pid == 0)
    {
        dup2(input, 0); 
        // dup2(output, 1);

        execve("/usr/bin/php-cgi", (char**)args, environ);
    }
    else
    {
        waitpid(pid, NULL, 0);
        close(input);
        close(output);
    }




}