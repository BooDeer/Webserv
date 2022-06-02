#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>



int main()
{
  std::cout << "hna " << std::endl;
    struct stat fileStat;

  if(stat("test.html", &fileStat) < 0)
    {
      std::cerr << "error  " << std::endl;
    }
  if (!(fileStat.st_mode & S_IRUSR))
    std::cout << "Read permission ";
  // if (fileStat.st_mode & S_IWUSR)
  //   std::cout << "Write permission ";
  // if (fileStat.st_mode & !S_IXUSR)
  //   std::cout << "Exec permission";

}