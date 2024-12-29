#include <ios>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <iomanip>
#include <fstream>
#include <thread>
#include <chrono>
#include <sstream>
#include <string.h>
#include <vector>
#include <iterator>

/* file pointers at wrong positions (fixed)*/
// Implemented vectors to fix the problem in an alternate way
// Find a way to mark the task 

// Function prototypes
std::string argParse(std::string argv);

class Task{
    const std::string path = "C:\\Users\\Susha\\Documents\\Programming\\C++\\Projects\\Task Tracker\\tasks.json";
    int id;
    std::string description;
    std::string status =  "todo";
    std::string createdAt;
    std::string updatedAt;

    // Private functions
    void createTaskFile(){
        std::fstream file(path, std::ios::app);
        fileError();
        file.close();
    }

    void fileError(){
        std::fstream file(path);
        if(!file){
            std::cerr << "Error: Couldn't access task file!" << std::endl;
            file.close();
            exit(0);
        }
    }

    int fileEmpty(){
        std::fstream file(path, std::ios::app);
        if(file.peek() == EOF){
            return 1;
        }
        else {
            return 0;
        }
    }

    std::string date(){
        // std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t time = std::time(nullptr);
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        std::string date = oss.str();
        return date;
    }

    void getTaskId(){
        std::ifstream file(path, std::ios::in);
        fileError();
        std::string line;
        if(file.peek() != EOF){
            while(std::getline(file, line)){
                std::stringstream ss(line);
                std::string* str_buf = new std::string;
                ss >> *str_buf;
                if((*str_buf).compare("id:") == 0){
                    int task_id;
                    ss >> task_id;
                    if(ss){
                        id = task_id + 1;
                    }
                }
                delete str_buf;
            }
        }
        else {
            id = 1;
        }
        file.close();
    }

public:

    // Constructor
    Task(){
        createTaskFile();
        getTaskId();
    }

    // Destructor
    ~Task(){
        delete this;
    }

    // Member functions
    void addTask(std::string description){
        getTaskId();
        std::ofstream file(path, std::ios::app);
        file << "id: " << id << std::endl;
        file << "description: " << description << std::endl;
        file << "status: " << status << std::endl;
        updatedAt = createdAt = date();
        file << "createdAt: "  << createdAt << std::endl;
        file << "updatedAt: " << updatedAt << std::endl;
        file << std::endl;
        file.close();
        std::clog << "Task added successfully!" << std::endl;
    }

    void updateTask(int task_id, std::string updated_description){
        std::ifstream ifile(path);
        fileError();
        if(!fileEmpty()){
            std::cerr << "Error: Task file is empty!" << std::endl;
            ifile.close();
            return;
        }
        std::vector<std::string> lines;
        std::vector<std::string>::iterator it;
        std::string line;
        while(std::getline(ifile, line)){
            lines.push_back(line);
        }
        for(it = lines.begin(); it < lines.end(); it++){
            std::stringstream ss(*it);
            ss.ignore(4, ' ');
            int n;
            if(ss >> n){
                if(n == task_id){
                    // const std::string desc = "description: "+updated_description;
                    *(it+1) = "description: "+updated_description;
                    std::ofstream ofile(path, std::ios::out | std::ios::trunc);
                    for(const auto& line_update: lines){
                        ofile << line_update << std::endl;
                    }
                    ofile.close();
                    std::clog << "Task updated successfully!" << std::endl;
                }
            }
        }
        ifile.close();
    }

    void deleteTask(int task_id){
        std::ifstream ifile(path);
        std::vector<std::string> lines;
        std::vector<std::string>::iterator it;
        std::string line;
        while(std::getline(ifile, line)){
            lines.push_back(line);
        }
        ifile.close();

        if(lines.empty()){
            std::cerr << "Error: File is empty!" << std::endl;
            return;
        }
        
        for(it = lines.begin(); it != lines.end(); it++){
            std::stringstream ss(*it);
            
            ss.ignore(4, ' ');
            int n;
            if(!(ss >> n)){
                continue;   // Ignore lines that don't contain a valid task ID
            }
            if(n == task_id){
                int startLine;
                const int numLinesToDelete = 6;
                if(std::distance(lines.begin(), it) <= 0){
                    startLine = 0;
                }
                else{
                    startLine = std::distance(lines.begin(), it);
                }
                int endLine = std::min(startLine + numLinesToDelete, static_cast<int>(lines.size()));

                if(startLine >= 0 && startLine < lines.size() && startLine < endLine && endLine <= lines.size()){
                    lines.erase(lines.begin() + startLine, lines.begin() + endLine);
                    std::ofstream ofile(path, std::ios::out | std::ios::trunc);
                    for(const auto& line: lines){
                        ofile << line << std::endl;
                    }
                    ofile.close();
                    std::clog << "Task deleted successfully!" << std::endl;
                    return;
                }
                else{
                    std::cerr << "Error: Invalid range for erase operation!" << std::endl;
                    return;
                }
            }
        }
        std::cerr << "Error: Task ID " << task_id << " not found!" << std::endl;
    }

    void markTask(int task_id, std::string argv){
        std::istringstream iss(argv);
        std::string;
        iss.ignore(4, '-');
        std::getline(iss, status);
    }

    void listTask();

};

int main(int argc, char* argv[]){

    Task task;

    for(int i = 1; i < argc; i++){
        if(strcmp("add", argv[i]) == 0){
            // std::cout << "argc: " << argc << std::endl;
            for(int j = (i+1); j < argc; j++){
                task.addTask(argv[j]);
            }
        }
        else if(strcmp("update", argv[i]) == 0){
            task.updateTask(atoi(argv[i+1]), argv[i+2]);
            i += 2;
        }
        else if(strcmp("delete", argv[i]) == 0){
            task.deleteTask(atoi(argv[i+1]));
            i++;
        }
        else if(strcmp("mark", argParse(argv[i]).c_str()) == 0){
            task.markTask(atoi(argv[i+1]), argv[i]);
            i++;
        }
    }
    
    return 0;
}

std::string argParse(std::string argv){
    std::istringstream ss(argv);
    std::string mark_buf = "";
    if(ss){
        std::getline(ss, mark_buf, '-');
    }
    return mark_buf;
}

// Main

// do{

    //     std::cout << "Task Tracker!" << std::endl;
    //     std::cout << "1. Create task" << std::endl;
    //     std::cout << "2. Update task" << std::endl;
    //     std::cout << "3. Delete task" << std::endl;
    //     std::cout << "4. Mark a task" << std::endl;
    //     std::cout << "5. List task" << std::endl;
    //     std::cout << "0. Exit" << std::endl;
    //     std::cout << "=> ";
    //     std::cin >> opt;
    //     switch(opt){
    //         case 1:
    //             task.addTask(n, argv[n]);
    //     }
    // } while(opt != 0);