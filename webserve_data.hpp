#ifndef WEBSERVE_DATA_HPP
#define WEBSERVE_DATA_HPP

#include "webserv.hpp"//
#include <iostream>
#include <iostream>
#include <vector>
#include <sys/time.h>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

struct data
{
    public:
    int id; // id for request
   // std::ofstream fd_file; // file stream
	int	_fileFd;
    bool is_header; // check is header
    //bool remove; // remove from header
    std::string _fileName;
    int client_socket; // client id socket 
    int server_socket; //  server is socket
    std::string path; 
    std::string method;
    std::string host;
    unsigned long long lenth; // Content-Length:
    long long size_read_complet;
    std::string Cookie;
    std::string type;
    std::string referer;
    int status_code;
    Locations  location;
    std::string root_cgi; // path of the cgi
    std::string paramter;
    std::string extension;
    ServerBlock config_block; // server block config
    unsigned short  port; // port fe
    std::string ip_server_name;
    
    public:
    data()
    {
        // set all default
        this->id  = 0;
        this->client_socket = 0;
        this->server_socket = 0;
        this->status_code = 0;
        this->lenth = 0;
        this->is_header = false;
        this->size_read_complet = 0;
       // this->remove = false;
    }
    data& operator=(data &o)
    {
        this->id = o.id;
        this->_fileName = o._fileName;
		this->_fileFd = open(o._fileName.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
        this->client_socket = o.client_socket;
        this->server_socket = o.server_socket;
        this->path = o.path;
        this->method = o.method;
        this->host = o.host;
        this->lenth = o.lenth;
        this->referer = o.referer;
        this->status_code = o.status_code;
        this->paramter = o.paramter;
        this->extension = o.extension;
		return *this;
    }
    void create_file(int fd_socket, int client_socket); // create file 
    ~data()
    {
         this->id  = 0;
        this->client_socket = 0;
        this->server_socket = 0;
        this->status_code = 0;
        is_header = false;
        //remove = false;
      //  close(_fileFd);
    }
    
};

class response;

class errors
{
    public:
    //std::string error_page_fd;
    std::string phrase;
    std::string error_header;
    bool you_can_genrate; // if not default page error you can genrate one
    errors()
    {
        you_can_genrate =  false;
    }
    errors(std::string err, std::string number)
    {
        this->phrase = err;
        this->error_header = number;

        if(error_header[0] == '2')
        {
            this->you_can_genrate = false;
        }
        else 
             this->you_can_genrate = true;
    }
    errors(const errors &err)
    {
        *this = err;
    }
    errors &operator=(const errors &err)
    {
        // this->err_number = err.err_number;
        //this->error_page_fd = err.error_page_fd;
        this->phrase =  err.phrase;
        this->you_can_genrate = err.you_can_genrate;
        return *this;
    }
    void generate_error(response &resp, std::map<std::string, std::string> &error);// generate reponce
    ~errors()
    {

    }
};

class response
{
    public:
        std::string header_resp;
        std::string status_code;
        std::string reason_phrase;
        std::map<std::string, errors> data_base;
        std::map<std::string, std::string> Common_types;
        int fd; //  for file want to send
        unsigned long long lenth;
        std::string output_file_name;
        std::string content_type; // content type from cgi
        

    public:
        void generate_response_header(const std::string &status, data &req);
        void send_response(data &req);
        void cgi_generate_response(data &req);
        void redirection_header_generate(data &req);

    response()
    {
        this->lenth = 0;
        // common status code
        data_base.insert(std::pair<std::string, errors>("400", errors("Bad Request", "400")));
        data_base.insert(std::pair<std::string, errors>("403", errors("Forbidden", "403")));
        data_base.insert(std::pair<std::string, errors>("404", errors("Not Found", "404")));
        data_base.insert(std::pair<std::string, errors>("405", errors("Method Not Allowed", "405")));
        data_base.insert(std::pair<std::string, errors>("413", errors("Payload Too Large", "413")));
        data_base.insert(std::pair<std::string, errors>("200", errors("OK", "200")));
        data_base.insert(std::pair<std::string, errors>("300", errors("Multiple Choices", "300")));
        data_base.insert(std::pair<std::string, errors>("301", errors("Moved Permanently", "301")));
        data_base.insert(std::pair<std::string, errors>("302", errors("Found", "302")));
        data_base.insert(std::pair<std::string, errors>("303", errors("See Other", "303")));
        data_base.insert(std::pair<std::string, errors>("304", errors("Not Modified", "304")));
        data_base.insert(std::pair<std::string, errors>("307", errors("Temporary Redirect", "307")));
        data_base.insert(std::pair<std::string, errors>("308", errors("Permanent Redirect", "308")));
        // 5xx
        data_base.insert(std::pair<std::string, errors>("500", errors("Internal Server Error", "500")));



        // common type
        Common_types.insert(std::pair<std::string, std::string>(".aac", "audio/aac"));
        Common_types.insert(std::pair<std::string, std::string>(".abw", "application/x-abiword"));
        Common_types.insert(std::pair<std::string, std::string>(".arc", "application/x-freearc"));
        Common_types.insert(std::pair<std::string, std::string>(".avif", "image/avif"));
        Common_types.insert(std::pair<std::string, std::string>(".avi", "video/x-msvideo"));
        Common_types.insert(std::pair<std::string, std::string>(".azw", "application/vnd.amazon.ebook"));
        Common_types.insert(std::pair<std::string, std::string>(".bin", "application/octet-stream"));
        Common_types.insert(std::pair<std::string, std::string>(".bmp", "image/bmp"));
        Common_types.insert(std::pair<std::string, std::string>(".bz", "application/x-bzip"));
        Common_types.insert(std::pair<std::string, std::string>(".bz2", "application/x-bzip2"));
        Common_types.insert(std::pair<std::string, std::string>(".cda", "application/x-cdf"));
        Common_types.insert(std::pair<std::string, std::string>(".csh", "text/css"));
        Common_types.insert(std::pair<std::string, std::string>(".csv", "text/csv"));
        Common_types.insert(std::pair<std::string, std::string>(".doc", "application/msword"));
        Common_types.insert(std::pair<std::string, std::string>(".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
        Common_types.insert(std::pair<std::string, std::string>(".eot", "application/vnd.ms-fontobject"));
        Common_types.insert(std::pair<std::string, std::string>(".epub", "application/epub+zip"));
        Common_types.insert(std::pair<std::string, std::string>(".gz", "application/gzip"));
        Common_types.insert(std::pair<std::string, std::string>(".gif", "image/gif"));
        Common_types.insert(std::pair<std::string, std::string>(".htm", "text/html"));
        Common_types.insert(std::pair<std::string, std::string>(".html", "text/html"));
        Common_types.insert(std::pair<std::string, std::string>(".ico", "image/vnd.microsoft.icon"));
        Common_types.insert(std::pair<std::string, std::string>(".ics", "text/calendar"));
        Common_types.insert(std::pair<std::string, std::string>(".jar", "application/java-archive"));
        Common_types.insert(std::pair<std::string, std::string>(".jpeg", "image/jpeg"));
        Common_types.insert(std::pair<std::string, std::string>(".jpg", "image/jpeg"));
        Common_types.insert(std::pair<std::string, std::string>(".js", "text/javascript"));
        Common_types.insert(std::pair<std::string, std::string>(".json", "application/json"));
        Common_types.insert(std::pair<std::string, std::string>(".mid", "audio/midi"));
        Common_types.insert(std::pair<std::string, std::string>(".midi", "audio/x-midi"));
        Common_types.insert(std::pair<std::string, std::string>(".mjs", "text/javascript"));
        Common_types.insert(std::pair<std::string, std::string>(".mp3", "audio/mpeg"));
        Common_types.insert(std::pair<std::string, std::string>(".mp4", "video/mp4"));
        Common_types.insert(std::pair<std::string, std::string>(".mpeg", "video/mpeg"));
        Common_types.insert(std::pair<std::string, std::string>(".mpkg", "application/vnd.apple.installer+xml"));
        Common_types.insert(std::pair<std::string, std::string>(".odp", "application/vnd.oasis.opendocument.presentation"));
        Common_types.insert(std::pair<std::string, std::string>(".ods", "application/vnd.oasis.opendocument.spreadsheet"));
        Common_types.insert(std::pair<std::string, std::string>(".odt", "application/vnd.oasis.opendocument.text"));
        Common_types.insert(std::pair<std::string, std::string>(".oga", "audio/ogg"));
        Common_types.insert(std::pair<std::string, std::string>(".ogv", "video/ogg"));
        Common_types.insert(std::pair<std::string, std::string>(".ogx", "application/ogg"));
        Common_types.insert(std::pair<std::string, std::string>(".opus", "audio/opus"));
        Common_types.insert(std::pair<std::string, std::string>(".otf", "font/otf"));
        Common_types.insert(std::pair<std::string, std::string>(".png", "image/png"));
        Common_types.insert(std::pair<std::string, std::string>(".pdf", "application/pdf"));
        Common_types.insert(std::pair<std::string, std::string>(".php", "application/x-httpd-php"));
        Common_types.insert(std::pair<std::string, std::string>(".ppt", "application/vnd.ms-powerpoint"));
        Common_types.insert(std::pair<std::string, std::string>(".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"));
        Common_types.insert(std::pair<std::string, std::string>(".rtf", "application/rtf"));
        Common_types.insert(std::pair<std::string, std::string>(".sh", "application/x-sh"));
        Common_types.insert(std::pair<std::string, std::string>(".svg", "image/svg+xml"));
        Common_types.insert(std::pair<std::string, std::string>(".swf", "application/x-shockwave-flash"));
        Common_types.insert(std::pair<std::string, std::string>(".tiff", "image/tiff"));
        Common_types.insert(std::pair<std::string, std::string>(".ts", "application/x-tar"));
        Common_types.insert(std::pair<std::string, std::string>(".ttf", "font/ttf"));
        Common_types.insert(std::pair<std::string, std::string>(".txt", "text/plain"));
        Common_types.insert(std::pair<std::string, std::string>(".vsd", "application/vnd.visio"));
        Common_types.insert(std::pair<std::string, std::string>(".wav", "audio/wav"));
        Common_types.insert(std::pair<std::string, std::string>(".webm", "video/webm"));
        Common_types.insert(std::pair<std::string, std::string>(".webp", "image/webp"));
        Common_types.insert(std::pair<std::string, std::string>(".woff", "font/woff"));
        Common_types.insert(std::pair<std::string, std::string>(".woff2", "font/woff2"));
        Common_types.insert(std::pair<std::string, std::string>(".xhtml", "application/xhtml+xml"));
        Common_types.insert(std::pair<std::string, std::string>(".xls", "application/vnd.ms-excel"));
        Common_types.insert(std::pair<std::string, std::string>(".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
        Common_types.insert(std::pair<std::string, std::string>(".xml", "application/xml"));
        Common_types.insert(std::pair<std::string, std::string>(".xul", "application/vnd.mozilla.xul+xml"));
        Common_types.insert(std::pair<std::string, std::string>(".zip", "application/zip"));
        Common_types.insert(std::pair<std::string, std::string>(".3gp", "video/3gpp"));
        Common_types.insert(std::pair<std::string, std::string>(".3g2", "video/3gpp2"));
        Common_types.insert(std::pair<std::string, std::string>(".7z", "application/x-7z-compressed"));
    }
    ~response()
    {
        data_base.clear();
        Common_types.clear();
    }

};







#endif