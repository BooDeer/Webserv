#include "../webserv.hpp"


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

void cgi_work(int fd_output_file, const char **args, const data &req, response &resp)
{
    int pid = fork(); //
    if(pid < 0)
        throw "500";
    int status;
    int input;
    input = open(req._fileName.c_str(), O_RDONLY, 0777);
    if (pid == 0)
    {
         std::stringstream b;
	    b << req.lenth;
	    std::string tmp;
	    b >> tmp;
        std::cout << "lengh file is ===> " << tmp << std::endl;
        setenv("GATEWAY_INTERFACE", "CGI/1.1",1);
        setenv("SERVER_PROTOCOL", "HTTP/1.1",1);
        setenv("REDIRECT_STATUS", "200",1); // change 
        setenv("SCRIPT_FILENAME", req.path.c_str(),1);
        setenv("REQUEST_METHOD", req.method.c_str(),1);
        setenv("CONTENT_LENGTH", tmp.c_str(),1); // test
        setenv("QUERY_STRING", req.paramter.c_str(),1);
        if(req.type.length() != 0)
        {
            std::cout << "type ===> " << req.type.c_str() << std::endl;
            setenv("CONTENT_TYPE", const_cast<char *>(req.type.c_str()), 1);
        }
        else if (resp.Common_types.find(req.extension) != resp.Common_types.end())
            setenv("CONTENT_TYPE", const_cast<char *>(resp.Common_types[req.extension].c_str()), 1);
        else
            setenv("CONTENT_TYPE", "application/octet-stream", 1);
        if(req.Cookie.length() != 0)
            setenv("HTTP_COOKIE", const_cast<char *>(req.Cookie.c_str()) ,1);
        // for(int i = 0; environ[i]; i++)
        //     std::cout << "env == > " <<  environ[i] << std::endl;
        std::cout << "duped file: " << req._fileName;
        dup2(input, 0);
        dup2(fd_output_file, 1);
        if(execve(req.root_cgi.c_str(), (char**)args, environ) < 0)
            exit(500);
    }
    else
    {
        waitpid(pid, &status, 0);
        if(status == 500)
            throw "500";
        close(input);
        close(fd_output_file);
    }
}
void parse_cgi_output(response &resp, std::string &name_of_file, data &req)
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
    int exist = stat(name_of_file.c_str(), &file_state);
   // std::cout << "sum ==> " << sum  << "filesize ==> " << file_state.st_size << std::endl;

    resp.lenth = file_state.st_size - sum;

    // std::cout << "size ==> " << resp.lenth << std::endl; 
}

void response::cgi_generate_response(data &req)
{
    struct stat fileStat;

    //  struct stat fileStat;
    int exist = stat(req.root_cgi.c_str(), &fileStat);
    std::cout << "======================================== cgi status ==========================" << std::endl;
    std::cout << "cgi exist ==> " << exist << std::endl;
    if(exist < 0 || !(fileStat.st_mode & S_IXUSR))
    {
        std::cout << "exit cgi problem" << std::endl;
        this->status_code = "500";
        this->reason_phrase = this->data_base[status_code].phrase;
        req.root_cgi.clear();
        this->header_resp = "HTTP/1.1 " + status_code + " " + reason_phrase + "\r\n\r\n";
        return ;
    }
    std::cout << "======================================== end cgi ==========================" << std::endl;
    std::string name = generate_name_of_file("cgi_output");
    const char *arg[3] = {const_cast<char *>(req.root_cgi.c_str()), const_cast<char *>(req.path.c_str()), NULL};
    int output = open(name.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0600);
    try
    {
        cgi_work(output, arg, req, *this);
        std::cout << "parse cgi done" << std::endl;
        parse_cgi_output(*this, name, req);
    }
    catch(char *err)
    {
        std::cerr <<  "error in cgi: " << err <<  '\n';
        status_code = err;
    }
    // //generate_name_of_file_of_the_named_file_of_that_one_named_file();
    this->header_resp = "HTTP/1.1 " + status_code + " " + reason_phrase + "\r\n"; // first line for response
    std::cout << "first line in header >>>>>> " << header_resp << std::endl;
    if(status_code[0] == '2')
    {
        this->output_file_name = generate_name_of_file("file_send");
        std::cout << "output_file_name: ===> " << this->output_file_name << std::endl;
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
        std::cout << " create file done" << std::endl;
    }
    struct stat file_state;
    stat(output_file_name.c_str(), &file_state);
    this->lenth = file_state.st_size;
        std::cout << std::endl;
   
    // 1.test cgi
    // 2.parse cgi
    // 3. test upload file
    // 4.test problem for responce
    //5. delete 
    // 6.bonus
    // this->header_resp.append("Server: webserv\r\n");


}

// int main()
// {
//     struct stat fileStat;


//     int exist = stat("test.sh", &fileStat);
//     // if(exit < 0)
//     //     this->status_code = "500";

//     if (!(fileStat.st_mode & S_IXUSR))
//         std::cout << "not exute" << std::endl;
//     else 
//         std::cout << "is\n";

// }