#pragma once

#include "../webserv.hpp"

void first_line(std::string line, data &save);
void parsing_header(std::stringstream &fs, data &d);
void check_url_path(data &req, std::vector<Locations> &conf);
// void response::send_response(data &req);
// void response::generate_response_header(const std::string &status, data &req);

