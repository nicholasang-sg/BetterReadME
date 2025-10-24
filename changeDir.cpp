#include <iostream>
#include <vector> 
#include <string>
#ifdef _WIN32
    #include <direct.h>  // filepath for Windows
#else
    #include <unistd.h>  // filepath for Linux/Mac
#endif

int changeDir(std::vector<std::string>& commands) {
    std::string &path = commands[1];
    if (chdir(path.c_str()) != 0) {
        std::cerr << "Error: Could not change directory to " << path << std::endl;
        return -1;
    }
    return 0;
}