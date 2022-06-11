#include "../webserv.hpp"
     #include <signal.h>
extern char **environ;
std::string generate_name_of_file(const std::string &gen)
{
    struct timeval	time;
	long long t;
    gettimeofday(&time, NULL);
    t = (time.tv_usec / 1000) + (time.tv_sec * 1000);
    std::stringstream b;
	b << t;
	std::string tmp;
	b >> tmp;
    std::string name  = "/tmp/" + gen + tmp + ".txt";
    return name;
}

void cgi_work(int fd_output_file, const char **args, data &req, response &resp)
{
    int pid = fork(); //
    if(pid < 0)
    {
        	if( req.config_block.__DefaultErrorpg.size() == 0)
					req.extension = ".html"; 
        throw "500";
    }
    int status;
    int input;
    
    input = open(req._fileName.c_str(), O_RDONLY, 0777);
    if (pid == 0)
    {
         std::stringstream b;
	    b << req.lenth;
	    std::string tmp;
	    b >> tmp;
        setenv("GATEWAY_INTERFACE", "CGI/1.1",1);
        setenv("SERVER_PROTOCOL", "HTTP/1.1",1);
        setenv("REDIRECT_STATUS", "200",1);
        setenv("SCRIPT_FILENAME", req.path.c_str(),1);
        setenv("REQUEST_METHOD", req.method.c_str(),1);
        setenv("CONTENT_LENGTH", tmp.c_str(),1);
        setenv("QUERY_STRING", req.paramter.c_str(),1);
        
        if(req.type.length() != 0)
        {
            setenv("CONTENT_TYPE", const_cast<char *>(req.type.c_str()), 1);
        }
        else if (resp.Common_types.find(req.extension) != resp.Common_types.end())
            setenv("CONTENT_TYPE", const_cast<char *>(resp.Common_types[req.extension].c_str()), 1);
        else
            setenv("CONTENT_TYPE", "application/octet-stream", 1);
        if(req.Cookie.length() != 0)
            setenv("HTTP_COOKIE", const_cast<char *>(req.Cookie.c_str()) ,1);
        if(req.cgiLocation.__UploadCGI.length() != 0) // upload location not found go to default
             setenv("UPLOAD_LCOATION", req.cgiLocation.__UploadCGI.c_str(),1);
        else
             setenv("UPLOAD_LCOATION", "/tmp/",1);

        dup2(input, 0);
        dup2(fd_output_file, 1);
        if(execve(req.root_cgi.c_str(), (char**)args, environ) < 0)
            exit(500);
    }
    else
    {
        waitpid(pid, &status, 0);
        if(status == 500)
        {
            	if( req.config_block.__DefaultErrorpg.size() == 0)
					req.extension = ".html"; 
            throw "500";
        }
        close(input);
        close(fd_output_file);
    }
}
void parse_cgi_output(response &resp, std::string &name_of_file)
{
    std::fstream fs(name_of_file);

    std::string line;
    int sum = 0;
    while (std::getline(fs, line))
    {
        if (line.length() == 1)
        {
            sum++;
            break;
        }
        sum += line.length() + 1;
    }
    struct stat file_state;
    stat(name_of_file.c_str(), &file_state);
    resp.lenth = file_state.st_size - sum;
}

void response::cgi_generate_response(data &req)
{
    struct stat fileStat;

    int exist = stat(req.root_cgi.c_str(), &fileStat);
    if(exist < 0 || !(fileStat.st_mode & S_IXUSR))
    {
        this->status_code = "500";
        this->reason_phrase = this->data_base[status_code].phrase;
        req.root_cgi.clear();
        this->header_resp = "HTTP/1.1 " + status_code + " " + reason_phrase + "\r\n\r\n";
        return ;
    }
    std::string name = generate_name_of_file("cgi_output");
    this->cgi_remove =  name;
    const char *arg[3] = {const_cast<char *>(req.root_cgi.c_str()), const_cast<char *>(req.path.c_str()), NULL};
    int output = open(name.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0600);
    try
    {
        cgi_work(output, arg, req, *this);
        parse_cgi_output(*this, name);
    }
    catch(const char *err)
    {
        status_code = err;
    }
    this->header_resp = "HTTP/1.1 " + status_code + " " + reason_phrase + "\r\n"; // first line for response
    if(status_code[0] == '2')
    {
        this->output_file_name = generate_name_of_file("file_send");
        std::ofstream fs(this->output_file_name);
        std::fstream fs2(name);

        fs << header_resp;
        header_resp.clear();
        std::stringstream b;
        b << this->lenth;
        std::string tmp;
        b >> tmp;
        this->header_resp = "Content-Length: " + tmp + "\r\n";
        fs << header_resp;
        std::string line;
        while(getline(fs2, line))
        {
            fs << line << "\n";
        }
    }
    struct stat file_state;
    stat(output_file_name.c_str(), &file_state);
    this->lenth = file_state.st_size;
}


