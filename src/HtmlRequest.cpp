#include "include/HtmlRequest.h"

HtmlRequest::HtmlRequest() : method(""), url(""){}

HtmlRequest::HtmlRequest(const html_comp& str) : method(""), url(""){
    std::string container;
    std::stringstream ss(str);

    ss >> container;
    this->setMethod(container);

    ss >> container;
    this->setUrl(container);
}

html_comp HtmlRequest::getMethod() const{
    return method;
}

void HtmlRequest::setMethod(html_comp method){
    this->method = method;
}

html_comp HtmlRequest::getUrl() const{
    return HTML_SRC_PATH+url;
}

void HtmlRequest::setUrl(html_comp url){
    this->url = url;
}

HtmlRequest& HtmlRequest::operator=(const html_comp& str){
    *this = HtmlRequest(str);
    return *this;
}

HtmlRequest::~HtmlRequest(){}

