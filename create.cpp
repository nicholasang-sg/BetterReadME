#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include <map>
#include <algorithm>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <pyconfig.h>
#include "Languages.h"

    // PyStatus status;
    // PyConfig config;

    // PyConfig_InitPythonConfig(&config);
    // config.home = Py_DecodeLocale("./python_embed", nullptr); // Set your folder

    // status = Py_InitializeFromConfig(&config);

    // if (PyStatus_Exception(status)) {
    //     Py_ExitStatusException(status);
    // }

    // PyRun_SimpleString(
    //     "from deep_translator import GoogleTranslator\n"
    //     "translated = GoogleTranslator(source='auto', target='fr').translate('Hello world')\n"
    //     "print(translated)"
    // );

    // Py_Finalize();
    // return 0;

void replaceInputWithLabel(const std::string& prompt, const std::string& label, std::string& input, std::vector<std::string>& sections) {
    std::cout << prompt << std::endl;
    std::cout << ">>> ";
    std::getline(std::cin, input);
    std::cout << "\033[2A";
    std::cout << "\r\033[K";
    std::cout << "\033[1B";
    std::cout << "\r\033[K";
    std::cout << "\033[1B";
    if (label == "Project Title") {
        std::cout << "\033[2A" << label << ": " << input << std::endl << std::endl;
        sections.push_back("# " + input + "\n");
        return;
    } else if (label == "Description") {
        std::cout << "\033[3A" << label << ": "<< input << std::endl << std::endl;
        sections.push_back(input + "\n\n");
        return;
    } else if (label == "License" && (input != "None" || input != "none")) {
        std::cout << "\033[3A" << label << ": " << input << std::endl << std::endl;
        sections.push_back("## License\n" + input + "\n\n");  
        return;

    } else {
        std::cout << "\033[3A" << label << ": " << input << std::endl << std::endl;
        sections.push_back("## " + label + "\n" + input + "\n\n");
        return;
    }
}

std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

void getSelectedLanguages(std::map<std::string, std::string>& supportedLanguages, std::vector<std::string>& selectedLanguages) {
    std::cout << "Select the languages for your project (One ReadME for each language)" << std::endl;
    std::cout << "Select 'help' to view all language and their code" << std::endl;
    std::cout << "Select 'add' to add a language" << std::endl;
    std::cout << "Select 'remove' to remove a language" << std::endl;
    std::cout << "Select 'confirm' to proceed to generate your ReadMEs" << std::endl;

    std::vector<std::string> commands;
    std::string inputCommand;
    std::string token;

    while (true) {
        commands.clear();
        std::cout << ">>> ";
        std::getline(std::cin, inputCommand);

        std::istringstream iss(inputCommand);
        while(iss >> token){
            commands.push_back(token);
        }

        if (commands[0] == "help") {
            for (const auto& lang : supportedLanguages) {
                std::cout << lang.first << " : " << lang.second << std::endl;
            }
        } else if (commands[0] == "add") {
            if (commands.size() < 2) {
                std::cout << "Error: 'add' command requires a language name argument." << std::endl;
                continue;
            }

            std::string inputLang = toLower(commands[1]);
            std::string matchedLang;
            bool found = false;

            for (const auto& [name, code] : supportedLanguages) {
                if (toLower(name) == inputLang || toLower(code) == inputLang) {
                    matchedLang = name; 
                    found = true;
                    break;
                }
            }

            if (!found) {
                std::cout << "Error: Language not supported." << std::endl;
                continue;
            }

            if (std::find(selectedLanguages.begin(), selectedLanguages.end(), matchedLang) != selectedLanguages.end()) {
                std::cout << "Error: " << matchedLang << " is already in your selected languages." << std::endl;
            } else {
                selectedLanguages.push_back(matchedLang);
                std::cout << matchedLang << " added successfully." << std::endl;
            }

        } else if (commands[0] == "remove") {
            if (commands.size() < 2) {
                std::cout << "Error: 'remove' command requires a language name argument." << std::endl;
                continue;
            }

            std::string inputLang = toLower(commands[1]);
            std::string matchedLang;
            bool found = false;

            for (const auto& lang : selectedLanguages) {
                if (toLower(lang) == inputLang || toLower(supportedLanguages[lang]) == inputLang) {
                    matchedLang = lang; 
                    found = true;
                    break;
                }
            }

            if (!found) {
                std::cout << "Error: Language not found in your selected languages." << std::endl;
                continue;
            }

            selectedLanguages.erase(
                std::remove(selectedLanguages.begin(), selectedLanguages.end(), matchedLang),
                selectedLanguages.end()
            );

            std::cout << matchedLang << " removed successfully." << std::endl;
        } else if (commands[0] == "confirm") {
            break;
        } else {
            std::cout << "Invalid command." << std::endl;
        }
    }
}

int create(const std::string& readmePath) {
    std::vector<std::string> sections;
    std::string input;

    replaceInputWithLabel("Input Project Title: ", "Project Title", input, sections);
    replaceInputWithLabel("Description: ", "Description", input, sections);
    replaceInputWithLabel("Installation: ", "Installation", input, sections);
    replaceInputWithLabel("Usage: ", "Usage", input, sections);
    replaceInputWithLabel("Contributing: ", "Contributing", input, sections);
    replaceInputWithLabel("License: ", "License", input, sections);

    std::map<std::string, std::string> supportedLanguages = Translation::getSupportedLanguages();
    std::vector<std::string> selectedLanguages;
    getSelectedLanguages(supportedLanguages, selectedLanguages);

    for (const auto& lang : selectedLanguages) {
        std::string langCode = supportedLanguages[lang];
        std::string outputFilePath = readmePath + "/README_" + langCode + ".md";
        std::ofstream file(outputFilePath);

        if (!file) {
            std::cerr << "Error: Could not create file " << outputFilePath << std::endl;
            continue;
        }

        file << "![Build](https://img.shields.io/badge/build-passing-brightgreen)\n\n";

        for (const auto& section : sections) {
            std::string translatedSection = Translation::translateText(section, langCode);
            file << translatedSection << "\n\n";
        }

        file.close();
        std::cout << "README.md file created for " << lang << " at " << std::filesystem::absolute(outputFilePath).string() << std::endl;
    }

    return 0;
}
