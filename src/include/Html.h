#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#define APPROVED "200 OK"
#define NOT_FOUND "404 NOT FOUND"
#define BAD_REQUEST "400 BAD REQUEST"

#define HOME "html/home.html"
#define VIDEO "html/videoPlayer.html"
#define P_NOT_FOUND "html/404.html"

#define TEXT_FILE "text/html"
#define VIDEO_FILE "video/MP2T"

namespace html{
    void sendHTML(int const &socketFD, std::string const page, std::string status = APPROVED);
    int handleHTMLReq(int const &socketFD);
    std::string formatReq(std::string const page, std::string const content_type, int& length, std::string const status);
}