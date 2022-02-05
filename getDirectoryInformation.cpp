//
// Created by augustin on 05/02/2022.
//
#include <string>
#include <utility>
#include <vector>
#include <sys/stat.h>
#include <filesystem>
#include "getDirectoryInformation.h"

using namespace std;

getDirectoryInformation::getDirectoryInformation(string sPath){
    this->s_path = std::move(sPath);
    this->path = this->s_path.c_str();
}


/**
 * Function to check whether a directory exists or not.
 * It returns 1 if given path is directory and  exists
 * otherwise returns 0.
 */

int getDirectoryInformation::isDirectoryExists() {
    struct stat stats;

    stat(this->path, &stats);

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
int getDirectoryInformation::isFileExists() {
    FILE *fptr = fopen(this->path, "r");
    if (fptr == nullptr)
        return 0;
    fclose(fptr);
    return 1;
}

std::vector<std::string> getDirectoryInformation::getAllFileOfFolder(const std::string& path) {
    vector<string> all_files;
    for (const auto & file : std::filesystem::directory_iterator(path)){
        std::string file_name = file.path();
        const char *_path = file_name.c_str();
        if (!isDirectoryExists(_path)){
            all_files.push_back(file_name);
        }else{
            vector<string> _files = getAllFileOfFolder(file_name);
            for (const auto& file_path : _files)
                all_files.push_back(file_path);

        }
    }
    return all_files;
}

int getDirectoryInformation::isDirectoryExists(const char *path) {
    struct stat stats;

    stat(path, &stats);

    // Check for file existence
    if (S_ISDIR(stats.st_mode))
        return 1;
    return 0;
}
