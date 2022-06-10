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






int main()
{

  std::string tmp = "abcd";
  std::cout << tmp.erase(0,tmp.length());
}
