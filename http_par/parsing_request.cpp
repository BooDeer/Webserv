#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>

struct data
{
    int id; // id for request
    std::string path;
    std::string method;
    std::string host;
    std::string lenth;
    std::string referer;
    data()
    {
        id = -666;
    }
};

void first_line(std::string line, data &save) // ====> GET example.com HTTP/1.1
{
    // data save;
    std::istringstream iss(line);
	std::string     buff;
    // std::cout << line << std::endl;
    for(int i = 0; i < 3; i++)
    {
        iss >> buff;
        if(i == 0)
            save.method = buff;
        else if(i == 1)
            save.path = buff;
    }
    // std::cout << save.method << std::endl << save.path << std::endl;
}

void parsing_header(std::fstream &fs, data &d)
{
    // find host  Referer lenth
    std::string lines;
    while (std::getline(fs, lines))
    {
        if (lines.find("Host:") != std::string::npos) // Host: 127.0.0.1:2082
        {
            // std::cout << "Reached here ==> "  << lines << std::endl;
            d.host = lines.erase(0, strlen("Host: "));
            // std::cout << "line == >" << lines << std::endl;
        }
        else if (lines.find("Content-Length:") != std::string::npos) // Content-Length: 69
        {
            // std::cout << "Reached here ==> "  << lines << std::endl;
            // lines.erase(0, strlen("Content-Length: "));
            d.lenth =  lines.erase(0, strlen("Content-Length: "));
            // std::cout << "line == >" << lines << std::endl; 
        }
        else if (lines.find("Referer:") != std::string::npos) // Referer: http://127.0.0.1:2082/
        {
            // std::cout << "Reached here ==> "  << lines << std::endl;
            // lines.erase(0, strlen("Content-Length: "));
            d.referer = lines.erase(0, strlen("Referer: "));
            // std::cout << "line == >" << lines << std::endl; 
        }
            //std::cout << "size ==> " << lines.length() << std::endl;
        if (lines.length()) //TODO: check this.
        {
            break ;
        }
    }
}


/* 
int main()
{
    int fd = open("../just_test", O_RDONLY);

    char *test[10000];
    read(fd, test, 10000);
    std::cout << test << std::endl;
}
 */

int main() {

    std::fstream fs;
    data data_to_save;
    // fs.open ("ex_3", std::fstream::in | std::fstream::out | std::fstream::app);
    // fs.open ("../just_test", std::fstream::in | std::fstream::out | std::fstream::binary);
    fs.open ("../just_test", std::fstream::in | std::fstream::out);
    // fs.open ("POST_REQUEST", std::fstream::in | std::fstream::out | std::fstream::app);
    std::string line_save;
//    fs << "\r\n\r\n";
    std::getline(fs, line_save);
    first_line(line_save, data_to_save);
    parsing_header(fs, data_to_save); // Parsing the rest of the request. (Header)
  //  check_
}
// std::istream& safeGetline(std::istream& is, std::string& t)
// {
//     t.clear();

//     // The characters in the stream are read one-by-one using a std::streambuf.
//     // That is faster than reading them one-by-one using the std::istream.
//     // Code that uses streambuf this way must be guarded by a sentry object.
//     // The sentry object performs various tasks,
//     // such as thread synchronization and updating the stream state.

//     std::istream::sentry se(is, true);
//     std::streambuf* sb = is.rdbuf();
//     // char test[500];
//     // sb->sgetn(test, )
//     for(;;) {
//         int c = sb->sbumpc();
        
//         switch (c) {
//         case '\n':
//             return is;
//         case '\r':
//             std::cout << " yessss " << std::endl;
//             if(sb->sgetc() == '\n')
//                 sb->sbumpc();
//             return is;
//         case EOF:
//             // Also handle the case when the last line has no line ending
//             if(t.empty())
//                 is.setstate(std::ios::eofbit);
//             return is;
//         default:
//             t += (char)c;
//         }
//     }
// }
// int main()
// {
//     std::string path = "../just_test";  // insert path to test file here

//     std::ifstream ifs(path.c_str());
//     if(!ifs) {
//         std::cout << "Failed to open the file." << std::endl;
//         return EXIT_FAILURE;
//     }

//     int n = 0;
//     std::string t;
//     while(!safeGetline(ifs, t).eof())
//         ++n;
//     std::cout << "The file contains " << n << " lines." << std::endl;
//     return EXIT_SUCCESS;
// }
