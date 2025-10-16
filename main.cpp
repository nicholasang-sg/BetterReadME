#include <iostream>
#include <string>
#include <cstdlib> 

int main() {
    std::string command;

    while (true) {
        std::cout << "BetterReadME> "; 
        std::getline(std::cin, command); 

        if (command == "exit") { 
            break; 
        }

        if (std::system(command.c_str()) == -1) {
            std::cerr << "Error: Could not execute command." << std::endl;
        }
    }

    std::cout << "Exiting simple shell." << std::endl;
    return 0;
}