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
    bool random_data = overiteRandomData(path,fSize);
    bool zero_data = overiteZeroData(path,fSize);
    if (random_data && zero_data){
        return deleteFile(path, fSize);
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



void shredFile::addWrapped(const Napi::CallbackInfo& info){
     Napi::Env env = info.Env();
     if(info.Length() != 1){
        Napi::TypeError::New(env, "arg1::string path expected").ThrowAsJavaScriptException();
     }
    std::string path = info[0].ToString().Utf8Value();
    shredFile::securelyEraseFile(path.c_str());
    //return ;
}
Napi::Object shredFile::Init(Napi::Env env, Napi::Object exports)
{
  //export Napi function
  exports.Set("secure_erase", Napi::Function::New(env, shredFile::addWrapped));
  return exports;
}