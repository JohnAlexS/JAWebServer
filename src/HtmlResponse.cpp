#include "include/HtmlResponse.h"

HtmlResponse::HtmlResponse() : status(0), length(0), content_type(""), content(""){}
HtmlResponse::HtmlResponse(html_comp status) : status(status), length(0), content_type(""), content(""){}

HtmlResponse::HtmlResponse(html_comp status, int length, html_comp content, html_comp type) 
    : status(status), length(length), content_type(type), content(content){}

void HtmlResponse::setStatus(html_comp status){
    this->status = status;
}

void HtmlResponse::setContent(html_comp content){
    this->content = content;
    this->length = content.length();
}

void HtmlResponse::setContent(vChar& content){
    this->length = content.size();
    this->content = std::string(content.data(), length);
}

void HtmlResponse::setType(html_comp type){
    this->content_type = type;
}

html_comp HtmlResponse::getData() const{
    return "HTTP/1.1 " + status + 
           "\r\nContent-Type: " + content_type + 
           "\r\nContent-Length:" + std::to_string(length) + 
           "\r\n\r\n" + content;
}

HtmlResponse::~HtmlResponse(){}