//
// Created by augustin on 05/02/2022.
//
#include <napi.h>
#include <string>
#include <vector>

using namespace std;

namespace getDirectoryInformation{

    int isDirectoryExists(const std::string& path);
    int isFileExists(const std::string& path);
    std::vector<std::string> getAllFileOfFolder(const std::string& path);

    Napi::Boolean wrapperDirectoryExist(const Napi::CallbackInfo& info);
    Napi::Boolean wrapperFileExist(const Napi::CallbackInfo& info);
    auto wrapperFileOfFolder(const Napi::CallbackInfo& info);

    Napi::Object Init(Napi::Env env, Napi::Object exports);
    NODE_API_MODULE(getDirectoryAddOn, Init);
}

