#include "Html.h"
#include "BetterSockets.h"
#include "Utils.h"
#include "FileSystem.h"

using namespace html;

std::string html::formatReq(std::string const page, std::string const status){
    return "HTTP/1.1 " + status + "\r\nContent-Length:" + std::to_string(page.length()) + "\r\n\r\n" + page;
}

void html::sendHTML(std::string page, int const &socketFD, std::string status){
    std::ostringstream content;
    std::ifstream info;

    info.open(page);
    if((info.rdstate() & std::ifstream::failbit) != 0){
        throw std::runtime_error("Failed to open file" + page);
    }

    content << info.rdbuf();
    
    std::string req = formatReq(content.str(), status);
    bsock::sendToSocket(socketFD, req);
}

int html::handleHTMLReq(int const &socketFD){
    std::string request;

    try{
        request = bsock::readSocket(socketFD, 1024);
    }
    catch(...){
        return -1;    
    }

    std::string req_head = request.substr(0,3);

    
    switch (utils::hash(req_head.c_str())){
        case utils::hash("GET"):
            try{
                sendHTML(FileSys::extractFilePath(request), socketFD);
            }
            catch(std::runtime_error& e){
                std::cout << "Couldn't open file: " << e.what() << std::endl;
                sendHTML("html/404.html", socketFD, NOT_FOUND);
            }

            break;
        
        default:
            return -1;
    }

    return 0;
}