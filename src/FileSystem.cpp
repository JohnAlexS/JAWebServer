#include <FileSystem.h>

std::string FileSys::extractFilePath(std::string& chunk){
    std::string path;
    size_t i = chunk.find('/');
    
    while(chunk.at(i) != ' ' && chunk.at(i) != '.'){
        path += chunk.at(i);
        i++;
    }

    if(path == "/"){
        return "html/home.html";
    }
    else{
        return "html" + path + ".html";
    }
}