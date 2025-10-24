#include <iostream>
#include <string>
#include <cstdlib>
#include <vector> 
#include <sstream>
#ifdef _WIN32
    #include <direct.h>  // filepath for Windows
    #define GETCWD _getcwd
    #define PATH_MAX 260
#else
    #include <unistd.h>  // filepath for Linux/Mac
    #include <limits.h>  
    #define GETCWD getcwd
#endif

#include "create.h"
#include "changeDir.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <pyconfig.h>


void welcomeMessage() {
    std::cout << "  ____       _   _            ____                _ __  __ _____ " << std::endl;
    std::cout << " | __ )  ___| |_| |_ ___ _ __|  _ \\ ___  __ _  __| |  \\/  | ____|" << std::endl;
    std::cout << " |  _ \\ / _ \\ __| __/ _ \\ '__| |_) / _ \\/ _` |/ _` | |\\/| |  _|  " << std::endl;
    std::cout << " |  _ \\ / _ \\ __| __/ _ \\ '__| |_) / _ \\/ _` |/ _` | |\\/| |  _|  " << std::endl;
    std::cout << " |____/ \\___|\\__|\\__\\___|_|  |_| \\_\\___|\\__,_|\\__,_|_|  |_|_____|" << std::endl << std::endl;

    std::cout << "Welcome to BetterReadME! Type 'exit' to quit. " << std::endl << std::endl;
}

int main() {
    PyStatus status;
    PyConfig config;

    // Initialize config
    PyConfig_InitPythonConfig(&config);
    config.home = Py_DecodeLocale("python_embed", NULL); // embedded Python folder

    // Initialize Python
    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        Py_ExitStatusException(status);
        return 1;
    }

PyRun_SimpleString(R"(
print("All available languages downloaded and installed!")
)");


    // Finalize Python
    Py_Finalize();
    PyConfig_Clear(&config);

    return 0;

    std::vector<std::string> commands;
    std::string inputCommand;
    std::string token;

    // Get current directory
    char buffer[FILENAME_MAX];

    welcomeMessage();

    while (true) {
        // Flushes input buffer
        commands.clear();

        // Gets current working directory at current REPL Loop instance 
        const char* cwd = GETCWD(buffer, sizeof(buffer));
        if (cwd == nullptr) {
            perror("Error getting current directory");
        }

        // Display prompt and get user input
        std::cout << "[BetterReadME] " << cwd << ">>> "; 
        std::getline(std::cin, inputCommand);
        
        // Seperate input command into tokens
        std::istringstream iss(inputCommand);
        while(iss >> token){
            commands.push_back(token);
        }

        if (commands.empty()){
            std::cout << "Please input something" << std::endl;
            continue;
        }

        if (commands[0] == "exit") { 
            break; 
        }

        if (commands[0] == "create") {
            if (commands.size() < 2) {
                std::cerr << "Error: 'create' command requires a directory path argument." << std::endl;
                continue;
            }
            std::string &directory = commands[1];
            create(directory);
            continue;
        }

        if (commands[0] == "cd") {
            if (commands.size() < 2) {
                std::cerr << "Error: 'cd' command requires a directory path argument." << std::endl;
                continue;
            }
            changeDir(commands);
            continue;
        }

        if (std::system(commands[0].c_str()) == -1) {
            std::cerr << "Error: Could not execute command." << std::endl;
        }
    }

    std::cout << "Exiting BetterReadME shell." << std::endl;
    return 0;
}