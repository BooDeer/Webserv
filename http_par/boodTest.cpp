#include <iostream>     
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
     #include <fcntl.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
  #include <sys/time.h>


struct bod {
  // std::string name;
  std::fstream test;
  bod()
  {
    std::cout << "Reacehd here" << std::endl;
  }
  // int  a;
  void create_file()
  {
    test.open("brah.txt",  std::fstream::out);

  }
};
int main()
{
    // bod te;
    // {
    //   te.create_file();
    // }
    // te.test << " reset" << std::endl;
	std::cout << 0777 << std::endl;
	int fd = open("/tmp/BoodTest", O_RDWR | O_CREAT, 0666);
	write(fd, "best", 5);
}
