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


#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <ftw.h>
#include <unistd.h>

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    if (rv) // remove this perror (errno not allowed)
        perror(fpath);

    return rv;
}

int rmrf(const char *path)
{
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

int main()
{





  int i = rmrf("./boodTestfile101");
  std::cout << "i =  " <<  i << std::endl;
}
