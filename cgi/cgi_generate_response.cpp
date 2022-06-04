#include "../webserv.hpp"


void response::cgi_generate_response(data &req)
{
    struct stat fileStat;

     struct stat fileStat;

    int exist = stat(req.root_cgi.c_str(), &fileStat);
    if(exit < 0)
        this->status_code = "500";
    if (!(fileStat.st_mode & S_IXUSR))
        this->status_code = "500";



    this->header_resp = "HTTP/1.1 " + status_code + " " + reason_phrase + "\r\n"; // first line for response
    this->header_resp.append("Server: webserv\r\n");
    
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