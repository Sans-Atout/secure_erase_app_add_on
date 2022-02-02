#include <iostream>
#include <ostream>
#include <vector>
#include <string>
#include "argparse/argparse.hpp"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <filesystem>
#include <cstdio>

using namespace std;


int isDirectoryExists(const char *path);
int isFileExists(const char *path);
int isFileExistsAccess(const char *path);
int isFileExistsStats(const char *path);
vector<string> getAllFileOfFolder(string path);


int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("Secure Erase App");
    program .add_argument("-p","--path")
            .required()
            .help("folder to delete");

    program .add_argument("--verbose")
            .help("increase output verbosity")
            .default_value(false)
            .implicit_value(true);

    try {
    program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }
    
    string pathToDelete = program.get<std::string>("--path");
    const char *_path = pathToDelete.c_str();
    
    if(isDirectoryExists(_path)){
        std::cout << " Folder selected : " << pathToDelete << std::endl;
        vector<string> files = getAllFileOfFolder(pathToDelete);
        int _id = 0;
        int _length = files.size();
        string _lengthFormating = to_string(to_string(_length).size());
        string log_line = " (%"+_lengthFormating+"d/"+to_string(_length)+") %s\n" ;
        printf(" We found %d files in this folder\n", _length);
        printf(" Starting the secure deletion of this files\n");

        for (string _fPath:files){
            string fName = _fPath.substr(pathToDelete.size(),_fPath.size()); 
            printf(log_line.c_str(),_id, fName.c_str());
            _id++;
        }
    }else{
        if (isFileExists(_path)){

        }else{
            std::cout << pathToDelete << " does not exist !" << std::endl;
            return 1;
        }
       
    }
    
}


/**
 * Function to check whether a directory exists or not.
 * It returns 1 if given path is directory and  exists 
 * otherwise returns 0.
 */
int isDirectoryExists(const char *path)
{
    struct stat stats;

    stat(path, &stats);

    // Check for file existence
    if (S_ISDIR(stats.st_mode))
        return 1;
    return 0;
}

/**
 * Function to check whether a file exists or not.
 * It returns 1 if file exists at given path otherwise
 * returns 0.
 */
int isFileExists(const char *path)
{
    // Try to open file
    FILE *fptr = fopen(path, "r");

    // If file does not exists 
    if (fptr == NULL)
        return 0;

    // File exists hence close file and return true.
    fclose(fptr);

    return 1;
}

/**
 * Function to check whether a file exists or not using 
 * access() function. It returns 1 if file exists at 
 * given path otherwise returns 0.
 */
int isFileExistsAccess(const char *path)
{
    // Check for file existence
    if (access(path, F_OK) == -1)
        return 0;

    return 1;
}

/**
 * Function to check whether a file exists or not using
 * stat() function. It returns 1 if file exists at 
 * given path otherwise returns 0.
 */
int isFileExistsStats(const char *path)
{
    struct stat stats;

    stat(path, &stats);

    // Check for file existence
    if (stats.st_mode & F_OK)
        return 1;

    return 0;
}

vector<string> getAllFileOfFolder(string path)
{
    vector<string> all_files;
    for (const auto & file : std::filesystem::directory_iterator(path)){
        std::string file_name = file.path();
        const char *_path = file_name.c_str(); 
        if (!isDirectoryExists(_path)){
            all_files.push_back(file_name);
        }else{
            vector<string> _files = getAllFileOfFolder(file_name);
            for (auto file_path : _files)
                all_files.push_back(file_path);

        }
    }
    return all_files;
}