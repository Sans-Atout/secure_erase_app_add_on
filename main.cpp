#include <iostream>
#include <vector>
#include <string>
#include "argparse/argparse.hpp"
#include "getDirectoryInformation.h"

using namespace std;

int main(int argc, char *argv[]) {
    argparse::ArgumentParser program("Secure Erase App","0.1.0.1");
    program .add_description("A secure erase app for linux based on shred program")
            .add_argument("-p","--path")
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

    auto pathToDelete = program.get<std::string>("--path");
    auto info = new getDirectoryInformation(pathToDelete);
    //const char *_path = pathToDelete.c_str();

    if(info->isDirectoryExists()){
        std::cout << " Folder selected : " << pathToDelete << std::endl;
        vector<string> files = getDirectoryInformation::getAllFileOfFolder(pathToDelete);
        int _id = 0;
        int _length = files.size();
        string _lengthFormating = to_string(to_string(_length).size());
        string log_line = " (%"+_lengthFormating+"d/"+to_string(_length)+") %s\n" ;
        printf(" We found %d files in this folder\n", _length);
        printf(" Starting the secure deletion of this files\n");

        for (const string& _fPath:files){
            string fName = _fPath.substr(pathToDelete.size(),_fPath.size());
            // command to do for erase files in linux basic
            // shred -zvu -n  7
            // source code is in shred.cpp
            printf(log_line.c_str(),_id, fName.c_str());
            _id++;
        }
    }else{
        if (info->isFileExists()){

        }else{
            std::cout << pathToDelete << " does not exist !" << std::endl;
            return 1;
        }

    }
    return 0;
}
