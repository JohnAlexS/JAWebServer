#pragma once

#include <string>
#include <sstream>
#include <fstream>

#define APPROVED "200 OK"
#define NOT_FOUND "404 NOT FOUND"
#define BAD_REQUEST "400 BAD REQUEST"

namespace html{
    void sendHTML(std::string page, int const &socketFD, std::string status = APPROVED);
    int handleHTMLReq(int const &socketFD);
    std::string formatReq(std::string page, std::string status);
}