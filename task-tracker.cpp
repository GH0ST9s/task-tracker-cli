#include <ios>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <sstream>
#include <string.h>
#include <vector>
#include <iterator>

// Bug: command arguments following "add" command works but are also added to the task-file as tasks.
// Formed JSON object //
// Form JSON array  //
// Implement how to write the object/array to file in a proper format   //
// Display tasks
// You can do it!

constexpr auto max_size = std::numeric_limits<std::streamsize>::max();

// Function prototypes
std::string argParse(std::string argv);

class Task{
    const std::string path = "tasks.json";
    int id;
    std::string description;
    std::string status =  "to-do";
    std::string createdAt;
    std::string updatedAt;

    // Private functions
    void createTaskFile(){
        std::ifstream ifile(path);
        if(!ifile || fileEmpty()){
            std::ofstream ofile(path, std::ios::out);
            ofile << "[" << std::endl;
            ofile << "]" << std::endl;
            ofile.close();
        }
        fileError();
        ifile.close();
    }

    // Error handling functions
    void fileError(){
        std::ifstream file(path);
        if(!file){
            std::cerr << "Error: Couldn't access task file!" << std::endl;
            file.close();
            exit(0);
        }
    }

    int fileEmpty(){
        std::ifstream file(path);
        // std::vector<std::string> lines;
        // readFile(lines);
        // display(lines);
        file.seekg(0);
        if(file.peek() == EOF){
            return 1;
        }
        else {
            return 0;
        }
    }

    void readFile(std::vector<std::string>& lines){
        std::ifstream ifile(path);
        fileError();
        std::string line;
        while(std::getline(ifile, line)){
            lines.push_back(line);
        }
        ifile.close();
    }

    void writeFile(std::vector<std::string>& lines){
        // Read from file
        std::string line;
        std::vector<std::string> file_lines;
        std::ifstream ifile(path);
        readFile(file_lines);
        ifile.close();

        // Formats the std::ostringstream/JSON Object
        std::ostringstream oss;
        for(const auto& line: lines){
            if(line == "[" || line == "]"){
                oss << line << std::endl;
            }
            else if(line == "{" || line == "}"){
                oss << std::setw(line.length()+2) << line << std::endl;
            }
            else{
                oss << std::setw(line.length()+4) << line << std::endl;
            }
        }

        // file_lines
        std::vector<std::string> new_lines;
        std::string temp;
        for(auto it = file_lines.begin(); it != file_lines.end();){
            if(*it == "]"){
                temp = *it;
                if(!file_lines.empty()){
                    file_lines.pop_back();
                }

                std::stringstream ss;
                if(ss << oss.str()){
                    while(std::getline(ss, line)){
                        new_lines.push_back(line);
                    }
                }
                new_lines.push_back(temp);
                break;
            }

    
            // for(char x: *it){
            //     if(x == '}' && *(it+1) != "]"){
            //         std::cout << "x: " << std::endl;
            //         *it = *it + ",";
            //     }
            // }

            ++it;
        }

        // Merges new_lines into file_lines
        file_lines.insert(file_lines.end(), new_lines.begin(), new_lines.end());
        for(auto it = file_lines.begin(); it != file_lines.end(); it++){
            for(char x: *it){
                if(x == '}' && *it != "  }," && *(it+1) != "]"){
                    *it = *it + ",";
                }
            }
        }

        // Write operation
        std::ofstream ofile(path, std::ios::trunc | std::ios::out);
        for(const auto& line: file_lines){
            ofile << line << std::endl;
        }
        ofile.close();

    }

    void removeLastComma(std::vector<std::string>& lines){
        for(auto it = lines.end()-2; it != lines.end(); it++){
            if(*it == *(lines.end()-2) && *it == "  }," && *(it+1) == "]"){
                std::string buff;
                buff = *it;
                buff.pop_back();
                *it = buff;
            }
        }
    }

    void updateFile(std::vector<std::string>& lines){
        std::ofstream ofile(path, std::ios::out | std::ios::trunc);
        fileError();
        for(const auto& line: lines){
            ofile << line << std::endl;
        }
        ofile.close();
    }

    void display(std::vector<std::string>& lines){
        for(const auto& line: lines){
            std::cout << line << std::endl;
        }
    }

    // void writeFile(std::ostringstream& oss){
    //     std::ofstream ofile(path, std::ios::app);
    //     std::vector<std::string> lines;
    //     readFile(lines);
    //     std::vector<std::string>::iterator it;
    //     for(const auto& line: lines){
    //         if(line == "["){
    //             ofile << line << std::endl;
    //             ofile << oss.str() << "]" << std::endl;
    //         }
    //     }
    //     // std::string last = *(lines.end()-1);
    //     // for(it = lines.begin(); it != lines.end(); it++){

    //     // }
    //     ofile.close();
    // }

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
        id = 1;
        if(file.peek() != EOF){
            while(std::getline(file, line)){
                std::stringstream ss(line);
                std::string str_buf;
                ss >> str_buf;
                if(str_buf == "\"id\":"){
                    int task_id;
                    ss >> task_id;
                    if(ss)
                        id = task_id + 1;
                }
            }
        }
        file.close();
    }

    void printTask(std::vector<std::string> lines){
        for(const auto& line: lines){
            std::cout << line << std::endl;
        }
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
        updatedAt = createdAt = date();

        std::vector<std::string> lines;
        lines.push_back("{");
        lines.push_back("\"id\": "+std::to_string(id));
        lines.push_back("\"description\": \""+description+"\"");
        lines.push_back("\"status\": \""+status+"\"");
        lines.push_back("\"createdAt\": \""+createdAt+"\"");
        lines.push_back("\"updatedAt\": \""+updatedAt+"\"");
        lines.push_back("}");


        // Adds comma to the end of line
        for(auto it = lines.begin(); it != lines.end()-1; it++){
            if(*it != "{" && *it != "["){
                if(*(it+1) != "}" && *(it+1) != "]"){
                    *it = *it + ",";
                }
            }
        }

        // File Write Operation
        writeFile(lines);
    
        std::clog << "Task added successfully! (ID: " << id << ")" << std::endl;
    }

    void updateTask(int task_id, std::string updated_description){
        if(fileEmpty()){
            std::cerr << "Error: Task file is empty!" << std::endl;
            return;
        }

        std::vector<std::string> lines;
        readFile(lines);

        std::vector<std::string>::iterator it;
        for(it = lines.begin(); it < lines.end(); it++){
            std::stringstream ss(*it);
            // ss.ignore(max_size, ' ');    // Didn't work
            std::string buf;
            ss >> buf;
            int n;
            if(ss >> n){
                if(n == task_id){
                    std::string line = *(it+1);
                    std::string space = std::string(4, ' ');
                    *(it+1) = space + "\"description\": \"" + updated_description + "\",";
                    *(it+4) = space + "\"updatedAt\": \"" + date() + "\"";
                    updateFile(lines);
                    std::clog << "Task updated successfully!" << std::endl;
                    return;
                }
            }
        }
        std::cerr << "Error: Task ID " << task_id << " not found!" << std::endl;
    }

    void deleteTask(int task_id){
        std::vector<std::string> lines;
        readFile(lines);
        if(lines.empty()){
            std::cerr << "Error: File is empty!" << std::endl;
            return;
        }

        std::vector<std::string>::iterator it;        
        for(it = lines.begin(); it != lines.end(); it++){
            std::stringstream ss(*it);
            
            std::string buf;
            ss >> buf;  // Unnecessary buffer
            int n;
            if(ss >> n){
                if(n == task_id){
                    int startLine;
                    const int numLinesToDelete = 7;
                    if(std::distance(lines.begin(), it-1) <= 0){
                        startLine = 0;
                    }
                    else{
                        startLine = std::distance(lines.begin(), it-1);
                    }
                    int endLine = std::min(startLine + numLinesToDelete, static_cast<int>(lines.size()));

                    if(startLine >= 0 && startLine < lines.size() && startLine < endLine && endLine <= lines.size()){
                        lines.erase(lines.begin() + startLine, lines.begin() + endLine);

                        // Lambda for ID Sorting
                        auto taskIdSort = [&task_id, &it, &lines](){
                            for(int i = 0; it != lines.end(); it++){
                                std::stringstream ss(*it);
                                std::string buff;
                                ss >> buff;
                                int n;
                                if(ss >> n){
                                    std::ostringstream oss;
                                    buff = *it;
                                    oss << std::setw(buff.length()-2) << "\"id\": " << task_id + i << ",";
                                    *it = oss.str();
                                    i++;
                                }
                            }
                        };
                        taskIdSort();
                        removeLastComma(lines);
                        
                        // Updates File
                        updateFile(lines);
                        std::clog << "Task deleted successfully!" << std::endl;
                        return;
                    }
                    else{
                        std::cerr << "Error: Invalid range for erase operation!" << std::endl;
                        return;
                    }
                }
            }
        }
        std::cerr << "Error: Task ID " << task_id << " not found!" << std::endl;
    }

    void markTask(int task_id, std::string argv){
        std::istringstream iss(argv);
        std::string updated_status;
        iss.ignore(5);
        iss >> updated_status;
        
        std::vector<std::string> lines;
        readFile(lines);

        if(lines.empty()){
            std::cerr << "Error: File is empty!" << std::endl;
            return;
        }

        std::vector<std::string>::iterator it;
        for(it = lines.begin(); it != lines.end(); it++){
            std::istringstream iss(*it);
            // iss.ignore(4);   Didn't work
            std::string temp;
            iss >> temp;
            int no;
            if(iss >> no && no == task_id){
                std::string stat[3] = {"to-do", "in-progress", "done"};
                if(updated_status == stat[0] || updated_status == stat[1] || updated_status == stat[2]){
                    std::string space = std::string(4, ' ');
                    *(it+2) = space + "\"status\": \"" + updated_status + "\",";
                    *(it+4) = space + "\"updatedAt\": \"" + date() + "\"";
                    updateFile(lines);
                    std::clog << "Task status updated successfully!" << std::endl;
                    return;
                }
                else{
                    std::cerr << "Error: Invalid command!" << std::endl;
                    return;
                }
            }
        }
        std::cerr << "Error: Task ID " << task_id << " not found!" << std::endl;
    }

    void listTask(std::string task_status = "all"){
        if(task_status == "all" || task_status == "to-do" || task_status == "done" || task_status == "in-progress"){
            std::vector<std::string> lines;
            readFile(lines);
            if(task_status == "all"){
                printTask(lines);
            }
            else{
                std::vector<std::string>::iterator it;
                for(it = lines.begin(); it != lines.end(); it++){
                    std::istringstream iss(*it);
                    iss.ignore(15);
                    std::string c_status;
                    if(iss >> c_status){
                        c_status.pop_back();
                        c_status.pop_back();
                        // std::cout << "status: " << c_status << std::endl;
                        // std::cout << "t: " << task_status << std::endl;
                        if(c_status == task_status){
                            // std::cout << "status: " << c_status << std::endl;
                            // std::cout << "t: " << task_status << std::endl;
                            std::cout << *(it-2) << std::endl;
                            std::cout << *(it-1) << std::endl;
                            std::cout << *(it) << std::endl;
                            std::cout << *(it+1) << std::endl;
                            std::cout << *(it+2) << std::endl;
                            std::cout << std::endl;
                        }
                        else{
                            // std::cerr << "Error: Status not found!" << std::endl;    // Unnecessary ig
                        }
                    }
                }
                
            }
            return;
        }
        std::cerr << "Error: Invalid task status!" << std::endl;
    }

};

int main(int argc, char* argv[]){

    Task task;

    for(int i = 1; i < argc; i++){
        if(strcmp("add", argv[i]) == 0){
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
        else if(strcmp("list", argv[i]) == 0){
            if(argv[i+1] == nullptr){
                task.listTask();
            } 
            else{
                task.listTask(argv[i+1]);
                i++;
            }
        }
    }
    
    return 0;
}

std::string argParse(std::string argv){
    std::istringstream ss(argv);
    std::string mark_buf;
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