#include <iostream>
#include <string>
#include <fstream>
#include <vector>

void replaceInputWithLabel(const std::string& prompt, const std::string& label, std::string& input, std::vector<std::string>& sections) {
    std::cout << prompt << std::endl;
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

int create(const std::string& readmePath) {

    std::vector<std::string> sections;
    std::string input;
    std::ofstream file(readmePath + "/README.md");

    file << "![Build](https://img.shields.io/badge/build-passing-brightgreen)\n\n";
    std::cout << "===========================README Generator===========================\nPlease answer the following questions to generate your README.md file.\n" << std::endl;
    
    replaceInputWithLabel("Input Project Title: ", "Project Title", input, sections);

    replaceInputWithLabel("Description: (Inform your users what the project is about)", "Description", input, sections);

    replaceInputWithLabel("Installation: (Show your users how to install/set up your project)", "Installation", input, sections);

    replaceInputWithLabel("Usage: (Show your users how to use your project)", "Usage", input, sections);

    replaceInputWithLabel("Contributing: (Let your users know how they can contribute to your project)", "Contributing", input, sections);

    replaceInputWithLabel("License: (If none, type 'None') ", "License", input, sections);

    for (const auto& section : sections) {
        file << section;
    }

    if (!file) {
        std::cerr << "Error: Could not create README.md file " << std::endl;
        return -1;
    }
    file.close();
    std::cout << "README.md file created successfully." << std::endl;
    return 0;
}