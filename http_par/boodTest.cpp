#include <iostream>     
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
     #include <fcntl.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>

int main()
{
    int fd = open("../just_test", O_RDONLY);

    char test[200];
    read(fd, test, 200);
	std::string bood(test);
	if (bood.find("\r\n\r\n") != std::string::npos)
		std::cout << "Found it" << std::endl;
    write(1, test, 200 );
	// std::fstream	fs("../just_test", std::fstream::in | std::fstream::out | std::fstream::binary);
	// std::string buf;
	// for (int i = 0; i < 7; i++)
	// {
	// 	std::getline(fs, buf);
	// 	// fs >> buf;
	// 	if (buf.find("\r\n\r\n") != std::string::npos)
	// 		std::cout << "IT IS WOOOOOOOORKING" << std::endl;
	// 	else
	// 		std::cout << buf << std::endl;
	// }
}
