#pragma once

#include <fstream>
#include <fcntl.h>
#include <stdexcept>
#include <sstream>
#include <map>

#define APPROVED "200 OK"
#define NOT_FOUND "404 NOT FOUND"
#define BAD_REQUEST "400 BAD REQUEST"

namespace utils{

    namespace{
        #define HEADER "HTTP/1.1 "
        #define LENGTH_HEADER "\r\nContent-Length:"
        #define SEPERATOR "\r\n\r\n"
    }


    static void setNoBlock(int& socketFD);
    static void sendHTML(std::string page, std::string status, int socketFD);
    static int handleHTMLReq(int socketFD);
}

void utils::setNoBlock(int& socketFD){
    int flags = fcntl(socketFD, F_GETFL, 0);

    if(flags < 0){
        throw std::runtime_error("Unable to retrieve socket flags");
    }

    if(fcntl(socketFD, F_SETFL, flags | O_NONBLOCK) < 0){
        throw std::runtime_error("Unable to set socket flags");
    }
}

void utils::sendHTML(std::string page, std::string status, int socketFD){
    std::ostringstream content;
    std::ifstream info;

    info.open(page);
    if((info.rdstate() & std::ifstream::failbit) != 0){
        throw std::runtime_error("Failed to open file");
    }

    content << info.rdbuf();
    int length = content.str().length();

    std::string str = HEADER + status + LENGTH_HEADER + std::to_string(length) + SEPERATOR + content.str();

    if(send(socketFD, str.c_str(), str.length(), MSG_NOSIGNAL) < 0){
        std::cout << errno << std::endl;
        std::cout << socketFD << std::endl;
        std::cout << str << std::endl;
        throw std::runtime_error("Failed to send HTML to client");
    }
}

int utils::handleHTMLReq(int socketFD){
    char buf[1024] = {0};
    std::string request;

    if(read(socketFD, buf, 1023) < 0){
        if(!(errno == EAGAIN || errno == EWOULDBLOCK)){
                std::cout << errno << std::endl;
                throw std::runtime_error("Failed to read buffer");
        }
    }

    else{
        for(int i = 0; (buf[i] != ' ') && (buf[i] != '\0'); i++){
            request += buf[i];
        }

        if(request == "GET"){
            std::string addr;
            for(int i = 4; buf[i] != ' ' && (buf[i] != '\0'); i++){
                addr += buf[i];
            }

            if(addr == "/"){
                sendHTML(("html/default.html"), APPROVED, socketFD);
            }
            else{
                sendHTML(("html/404.html"), NOT_FOUND, socketFD);
                return -1;
            }
        }
    }

    return 0;
}