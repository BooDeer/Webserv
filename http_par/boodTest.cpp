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
  std::string name;
  bod()
  {
    std::cout << "Reacehd here" << std::endl;
  }
  // int  a;

};
int main()
{
// 	std::fstream fd_file1;
// 	std::fstream &fd_file2 = fd_file1;
    // std::ofstream bood("/tmp/BoodTest.txt", std::fstream::out);
    // bood << "test";
    // bood.close();
    // bood.open("/tmp/BoodTest.txt", )
    // for (int i = 0; i < 5; i++)
    // {
    //   bod lol;
    //   std::cout << &lol << std::endl;
    //   if (i == 3) {
    //     lol.name = "beep boop";
    //   }

    //   std::cout << lol.name << std::endl;
    // }
    std::fstream fd1, fd2;
    fd1 = fd2;
	// fd_file2 << fd_file1.rdbuf();
}
