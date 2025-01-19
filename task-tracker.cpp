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
#include <algorithm>

// Bug: command arguments following "add" command works but are also added to the task-file as tasks.
// Formed JSON object //
// Form JSON array  //
// Implement how to write the object/array to file in a proper format   //
// Display tasks
// Learn to make tables
// You can do it!

constexpr auto max_size = std::numeric_limits<std::streamsize>::max();

enum Label{
    NOTHING = 0, ID, DESCRIPTION, STATUS, CREATED, UPDATED
};

// Function prototypes
std::string argParse(std::string argv);

class Table{
    int id;
    std::string sId = "ID";
    std::string description = "Description";
    std::string status = "Status";
    std::string createdAt = "Created At";
    std::string updatedAt = "Updated At";

    std::vector<std::string> vDesc;
    std::vector<std::string> vStatus;
    std::vector<std::string> vCreated;
    std::vector< std::string> vUpdated;

    int ID_SIZE = sId.size();
    int DESC_SIZE = description.size();
    int STATUS_SIZE = status.size();
    int CREATEDAT_SIZE = createdAt.size();
    int UPDATEDAT_SIZE = updatedAt.size();
public:

    Table(std::vector<std::string>& lines){
        for(auto it = lines.begin(); it != lines.end(); it++){
            std::istringstream iss(*it);
            std::string temp;
            std::string buffer;
            iss >> temp;
            if(temp == "\"description\":"){
                iss.ignore(2);
                iss >> buffer;
                
                iss.str().pop_back();
                iss.str().pop_back();
                iss.seekg(20);
                std::cout << "desc: " << iss.str() << std::endl;
                std::cout << "desc: " << buffer << std::endl;
            }
            // if(iss >> buffer){
            //     std::cout << "desc: " << buffer << std::endl;
            // }
        }
    }

    void printBorder(){
        std::cout << "+" << std::string(sId.size()+3, '-') << "+" << std::string(description.size()+5, '-') << "+" << std::string(status.size()+3, '-') << "+" << std::string(createdAt.size(), '-') << "+" << std::string(updatedAt.size(), '-') << std::endl;
    }

    void printHeader();
    void print();
};

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
    
    typedef std::vector<int> vInt;
    typedef std::vector<std::string> vString;
    void printTask(vInt& idV, vString& descriptionV, vString& statusV, vString& createdV, vString& updatedV){
        std::string strId = "id";
        std::string strDescription = "description";
        std::string strStatus = "status";
        std::string strCreated = "created at";
        std::string strUpdated = "updated at";
        int id_size = strId.size()+3;
        int description_size = strDescription.size()+10;
        int status_size = strStatus.size()+6;
        int created_size = strCreated.size()+10;
        int updated_size = strUpdated.size()+10;

        // Finds max description.size()
        for(auto it = descriptionV.begin(); it < descriptionV.end(); it++){
            int i = (*it).size();
            if(description_size <= i){
                description_size = i+1;
            }
        }

        // Prints Table
        using std::setw;
        char plus = '+';
        auto dash = [](int n, char ch = '-'){
            return std::string(n, ch);
        };

        auto printBorder = [&](){
            std::cout << "+" << dash(id_size) << plus << dash(description_size) << plus << dash(status_size) <<
            plus << dash(created_size) << plus << dash(updated_size) << plus << std::endl;
        };

        auto printHeader = [&](){
            std::cout << std::left << "|" << setw(id_size) << strId << "|" << setw(description_size) << strDescription <<
            "|" << setw(status_size) << strStatus << "|" << setw(created_size) << strCreated << "|" << setw(updated_size) << strUpdated << "|" << std::endl;

        };

        auto printBody = [&](){
            
        };

        printBorder();
        printHeader();
        printBorder();
        for(size_t i = 0; i < idV.size(); i++){
            std::cout << std::left << "|" << setw(id_size) << idV[i] << "|" << setw(description_size) << descriptionV[i] <<
            "|" << setw(status_size) << statusV[i] << "|" << setw(created_size) << createdV[i] << "|" << setw(updated_size) << updatedV[i] << "|" << std::endl;
        }
        printBorder();

        // for(size_t i = 0; i < idV.size(); i++){
        //     std::cout <<
        //         idV[i] << ". " << descriptionV[i] << " " <<
        //         statusV[i] << " " << createdV[i] << " " << updatedV[i] << std::endl;
        // }
    }

    Label enumCheck(std::string& temp){
        enum Label label;
        if(temp == "id"){
            label = ID;
        }
        else if(temp == "description"){
            label = DESCRIPTION;
        }
        else if(temp == "status"){
            label = STATUS;
        }
        else if(temp == "createdAt"){
            label = CREATED;
        }
        else if(temp == "updatedAt"){
            label = UPDATED;
        }
        else{
            return NOTHING;
        }
        return label;
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

            size_t listSize = 0;

            std::vector<int> idV;
            std::vector<std::string> descriptionV;
            std::vector<std::string> statusV;
            std::vector<std::string> createdV;
            std::vector<std::string> updatedV;
            for(auto it = lines.begin(); it != lines.end(); it++){
                std::istringstream iss(*it);
                std::string temp;
                iss.ignore(5);
                if(iss >> temp){
                    temp.pop_back();
                    temp.pop_back();
                }
                
                enum Label label;
                label = enumCheck(temp);
                std::string labelValue;
                auto parseLabelValue = [&iss, &labelValue](std::vector<std::string>& labelVector, bool pop = true){
                    std::getline(iss.ignore(2), labelValue);
                    if(pop){
                        labelValue.pop_back();
                        labelValue.pop_back();
                    }
                    else{
                        labelValue.pop_back();
                    }

                    labelVector.push_back(labelValue);
                };
                switch(label){
                    case ID:
                        std::getline(iss.ignore(1), labelValue);
                        labelValue.pop_back();
                        idV.push_back((stoi(labelValue)));
                        break;
                    case DESCRIPTION:
                        parseLabelValue(descriptionV);
                        ++listSize;
                        break;
                    case STATUS:
                        parseLabelValue(statusV);
                        break;
                    case CREATED:
                        parseLabelValue(createdV);
                        break;
                    case UPDATED:
                        parseLabelValue(updatedV, false);
                        break;
                    default:
                        break;
                }
            }

            if(task_status == "all"){
                printTask(idV, descriptionV, statusV, createdV, updatedV);
            }
            else{
                auto checkStatus = [&](){
                    vInt t_id;
                    vString t_description;
                    vString t_status;
                    vString t_created;
                    vString t_updated;
                    for(size_t i = 0; i < listSize; i++){
                        if(task_status == statusV[i]){
                            // check condition for the task status
                            t_id.push_back(idV[i]);
                            t_description.push_back(descriptionV[i]);
                            t_status.push_back(statusV[i]);
                            t_created.push_back(createdV[i]);
                            t_updated.push_back(updatedV[i]);
                        }
                    }
                    printTask(t_id, t_description, t_status, t_created, t_updated);
                };
                checkStatus();
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