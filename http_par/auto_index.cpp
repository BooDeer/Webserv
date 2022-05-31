#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>



void  auto_index() // pass class data
{
    // name_dir == path
    std::string name_dir = "test";
    std::string html_save = "<html>\n<head><title>Index of " + name_dir + "</title></head>\n<body>\n<h1>Index of " + name_dir+ "</h1><hr><pre><a href=\"../\">../</a>\n";
   std::fstream MyFile;
    MyFile.open("autoindex.txt", std::ios::out); // create uniq file // now just test
    MyFile << html_save;
    html_save.clear();
    //std::cout << html_save << std::endl;
    DIR *dir;

    dir = opendir("./");
    if (dir == NULL)
        throw "403";
    
    struct dirent *entry;

    struct stat st;

    while( (entry= readdir(dir)) != NULL)
    {
      if(entry->d_name[0] != '.')
      {
          std::string tmp(entry->d_name);
          stat(entry->d_name, &st);
        // struct tm * timeinfo = localtime(&st.st_ctime);
        //    std::cout << asctime(timeinfo);
        std::string tmp2(ctime(&st.st_ctime));
        tmp2.erase(tmp2.length()-1);
          if(tmp.length() < 33)
          {
            html_save = "<a href=\""+ tmp + "\">" +  tmp  + "</a>";
          }
          else
          {
              html_save = "<a href=\""+ tmp + "\">";
              for(int i = 0; i <  33; i++)
              {
                html_save.append(1, tmp[i]);
              }
               html_save.append("..&gt;</a>");
          }
            MyFile << html_save           <<  std::setw(60 - tmp.length())  << tmp2 << std::setw(10)  << st.st_size << std::endl;
          tmp.clear();
          tmp2.clear();
          html_save.clear();
      }

    }
    MyFile << "</pre><hr></body>\n</html>\n";

    closedir(dir);
}


int main()
{
    // DIR *dir;
    // struct dirent *ent;
    //
    // if ((dir = opendir ("./")) != NULL)
    // {
    //     while ((ent = readdir (dir)) != NULL)
    //     {
    //         std::string filename(ent->d_name);

    //         std::cout << filename << "\n";
    //     }
    //     closedir(dir);
    // }
    // DIR *dir;
    // struct dirent *entry;

    // struct stat st;
    // std::string tmp2(ctime(&st.st_ctime));
    // tmp2.erase(tmp2.length()-1);
    // std::cout << "testaa" <<  std::setw(tmp2.length() + 20 + 6) << tmp2 << std::endl;
    // std::cout << "aaaaaaaa" <<  std::setw(tmp2.length() + 20 + 8) << tmp2 << std::endl;
    // std::cout << "ddddddddddddd" <<  std::setw(tmp2.length() + 20 + 13) << tmp2 << std::endl;
    // std::cout << "testaaaaaaaaaaaaa" <<  std::setw(tmp2.length() + 20 + 17) << tmp2 << std::endl;
    auto_index();

    // std::cout << "testaa"           <<  std::setw(60 - 6)  << tmp2 << std::setw(10)  << "1337" << std::endl;
    // std::cout << "aaaaaaaa"         <<  std::setw(60 - 8)  << tmp2 << std::setw(10)  << "1337" << std::endl;
    // std::cout << "ddddddddddddd"    <<  std::setw(60 - 13) << tmp2 << std::setw(10)  << "1337" << std::endl;
    // std::cout << "testaaaaaaaaaaaa" <<  std::setw(60 - 16) << tmp2 << std::setw(10)  << "1337" << std::endl;


}
