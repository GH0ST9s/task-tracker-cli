#include <iostream>
#include <string>
#include <fstream>

int main(){

    const std::string path = "C:\\Users\\Susha\\Documents\\Programming\\C++\\Projects\\Task Tracker\\tasks.json";
    std::fstream file(path, std::ios::in | std::ios::app);
    std::string line;
    // file.seekg(-32, std::ios::end);
    std::cout << "p: " << file.tellg() << std::endl;
    while(std::getline(file, line)){
        std::cout << "p: " << file.tellg() << std::endl;
        std::cout << line << std::endl;

    }
    file.close();
    return 0;
}