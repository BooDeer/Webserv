#pragma once
struct data
{
    
    int id; // id for request
    char* path;
    char* method;
    char* host;
    char* body_path;
    char* response_path;
    char* content_length;
    char* content_type;
    char* referer;
    char* status_code;
    char* query_string;
    char* extension;
    // char* file_name; mazal mam7tajouch
    data()
    {
        id = -666; // hehe    
        this->path = (char *)"test.php";
        this->content_length = (char *)"";
        this->content_type = (char *)"multipart/form-data; boundary=----WebKitFormBoundaryrEvtmhEJEIqn43cE";
        this->status_code = (char *)"200";
        this->query_string = (char *)"user=Admin&pass=lcodepoincom1234";
        this->method = (char *)"GET";
        this->body_path = (char *)"body-msg";
        this->response_path = (char *)"respense.php";
    }
};
