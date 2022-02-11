//
// Created by augustin on 05/02/2022.
//
#include <napi.h>
#include <string>
#include <vector>
namespace shredFile{

    bool securelyEraseFile(const char *path);
    bool overiteZeroData(const char *path, unsigned long f_size);
    bool deleteFile(const char *path, unsigned long f_size);
    bool overiteRandomData(const char *path, unsigned long f_size);
    std::vector<std::string> getDirectoryAndFiles(std::string s);
    
    void addWrapped(const Napi::CallbackInfo& info);
    //Export API
    Napi::Object Init(Napi::Env env, Napi::Object exports);
    NODE_API_MODULE(addon, Init)

}
