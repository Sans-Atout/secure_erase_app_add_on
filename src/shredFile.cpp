/*
 * This shred file programm is inspired from the shred program of coresutils and the programm to the following link
 * https://codereview.stackexchange.com/questions/85573/a-program-to-shred-files
 * */
#include "shredFile.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>


#include <string>
#include <utility>
#include <sys/stat.h>

using namespace std;

const int DEFAULT_ITERATION = 5;

bool shredFile::overiteRandomData(const char *path, unsigned long f_size) {
    try {
        std::vector<unsigned char> buffer(f_size);
        std::fstream fwrite;

        fwrite.open(path, std::fstream::binary | std::fstream::out | std::fstream::in);
        //TODO implement error
        int i = DEFAULT_ITERATION;
        while (i--) {
            for (auto &element: buffer)
                element = std::rand() % 128;
            fwrite.seekp(0, std::ios::beg);
            fwrite.write((char *) &buffer[0], buffer.size());
            fwrite.flush();
        }
        fwrite.close();
        return true;
    }
    catch( const std::exception & ex ) {
        return false;
    }
}

bool shredFile::overiteZeroData(const char *path, unsigned long f_size) {
    try {
        std::vector<unsigned char> buffer(f_size);
        std::fstream fwrite;

        fwrite.open(path, std::fstream::binary | std::fstream::out | std::fstream::in);
        //TODO implement error
        int i = DEFAULT_ITERATION;
        for (auto &element: buffer) element = 0;
        fwrite.seekp(0, std::ios::beg);
        fwrite.write((char *) &buffer[0], buffer.size());
        fwrite.flush();
        fwrite.close();
        return true;
    }
    catch( const std::exception & ex ) {
        return false;
    }
}

bool shredFile::securelyEraseFile(const char *path) {
    auto fSize = std::filesystem::file_size(path) ;
    bool random_data = shredFile::overiteRandomData(path,fSize);
    bool zero_data = shredFile::overiteZeroData(path,fSize);
    if (random_data && zero_data){
        return shredFile::deleteFile(path, fSize);
    }else{
        return false;
    }
}


bool shredFile::deleteFile(const char *path, unsigned long f_size) {
    std::string _p = path;
    std::vector<std::string> vec = getDirectoryAndFiles(_p);
    std::string dirPath = vec[0];
    std::string _oldFileName = vec[1];
    std::string _newName;
    for (int i=0; i<_oldFileName.size(); i++) _newName += "0";
    while(_oldFileName.size() != 1 ){
        std::string oldPath = dirPath+_oldFileName;
        std::string newPath = dirPath+_newName;
        std::rename(oldPath.c_str(),newPath.c_str());
        //std::cout <<oldPath << "\t" << newPath << std::endl;

        _oldFileName  = _newName;
        _newName = "";
        for (int i=0; i<_oldFileName.size()-1; i++) _newName += "0";
    }
    std::string path_to_remove = dirPath+"0";
    std::remove(path_to_remove.c_str());
    return true;
}

std::vector<std::string> shredFile::getDirectoryAndFiles(std::string s) {
    std::string delimiter = "/";
    size_t pos = 0;
    std::vector<std::string> pathFiles;
    std::string token;
    std::string directory_path;

    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
        directory_path += token+'/';
    }

    pathFiles.push_back(directory_path);
    pathFiles.push_back(s);

    return pathFiles;
}



Napi::Boolean shredFile::secureEraseWrapper(const Napi::CallbackInfo& info){
     Napi::Env env = info.Env();
     if(info.Length() != 1){
        Napi::TypeError::New(env, "arg1::string path expected").ThrowAsJavaScriptException();
     }
    std::string path = info[0].ToString().Utf8Value();
    return Napi::Boolean::New(env, shredFile::securelyEraseFile(path.c_str()));
    //return ;
}


Napi::Object shredFile::Init(Napi::Env env, Napi::Object exports)
{
  //export Napi function
  exports.Set("secure_erase", Napi::Function::New(env, shredFile::secureEraseWrapper));
  exports.Set("dir_exist", Napi::Function::New(env, shredFile::wrapperDirectoryExist));
  exports.Set("file_exist", Napi::Function::New(env, shredFile::wrapperFileExist));

  return exports;
}


Napi::Boolean shredFile::wrapperDirectoryExist(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if(info.Length() != 1){
        Napi::TypeError::New(env, "arg1::string path expected").ThrowAsJavaScriptException();
    }
    std::string path = info[0].ToString().Utf8Value();
    return Napi::Boolean::New(env, shredFile::isDirectoryExists(path));
}

int shredFile::isDirectoryExists(const std::string& path) {
    struct stat stats;

    stat(path.c_str(), &stats);

    // Check for file existence
    if (S_ISDIR(stats.st_mode))
        return 1;
    return 0;
}

int shredFile::isFileExists(const std::string& path) {
    FILE *fptr = fopen(path.c_str(), "r");
    if (fptr == nullptr)
        return 0;
    fclose(fptr);
    return 1;
}

Napi::Boolean shredFile::wrapperFileExist(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 1){
        Napi::TypeError::New(env, "arg1::string path expected").ThrowAsJavaScriptException();
    }
    std::string path = info[0].ToString().Utf8Value();
    return Napi::Boolean::New(env , shredFile::isFileExists(path));
}