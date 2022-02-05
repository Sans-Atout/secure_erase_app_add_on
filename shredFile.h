//
// Created by augustin on 05/02/2022.
//

#ifndef SECURE_ERASE_APP_SHREDFILE_H
#define SECURE_ERASE_APP_SHREDFILE_H


#include <string>
#include <vector>

class shredFile {
public:
    static bool securelyEraseFile(const char *path);
private:
    static bool overiteZeroData(const char *path, unsigned long f_size);
    static bool deleteFile(const char *path, unsigned long f_size);
    static bool overiteRandomData(const char *path, unsigned long f_size);
    static std::vector<std::string> getDirectoryAndFiles(std::string s);
};


#endif //SECURE_ERASE_APP_SHREDFILE_H
