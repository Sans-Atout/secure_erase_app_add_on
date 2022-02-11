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

/**
 * Function to check whether a directory exists or not.
 * It returns 1 if given path is directory and  exists
 * otherwise returns 0.
 */

int getDirectoryInformation::isDirectoryExists(const std::string& path) {
    struct stat stats;

    stat(path.c_str(), &stats);

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
int getDirectoryInformation::isFileExists(const std::string& path) {
    FILE *fptr = fopen(path.c_str(), "r");
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
        if (!getDirectoryInformation::isDirectoryExists(_path)){
            all_files.push_back(file_name);
        }else{
            vector<string> _files = getDirectoryInformation::getAllFileOfFolder(file_name);
            for (const auto& file_path : _files)
                all_files.push_back(file_path);

        }
    }
    return all_files;
}

/*
void getDirectoryInformation::addWrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 1){
        Napi::TypeError::New(env, "arg1::string path expected").ThrowAsJavaScriptException();
    }
    std::string path = info[0].ToString().Utf8Value();
    getDirectoryInformation::securelyEraseFile();
    return true;
}
Napi::Object getDirectoryInformation::Init(Napi::Env env, Napi::Object exports)
{
    //export Napi function
    exports.Set("secure_erase", Napi::Function::New(env, example::addWrapped));
    return exports;
}*/