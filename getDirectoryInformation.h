//
// Created by augustin on 05/02/2022.
//
#include <string>
#include <vector>

using namespace std;
#ifndef SECURE_ERASE_APP_GETDIRECTORYINFORMATION_H
#define SECURE_ERASE_APP_GETDIRECTORYINFORMATION_H

class getDirectoryInformation {

    std::string s_path;
    const char *path;
public:

    int isDirectoryExists();
    int isFileExists();
    static std::vector<std::string> getAllFileOfFolder(const std::string& path);

    explicit getDirectoryInformation(string sPath);

    static int isDirectoryExists(const char *string);
};


#endif //SECURE_ERASE_APP_GETDIRECTORYINFORMATION_H
