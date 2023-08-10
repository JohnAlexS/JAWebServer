#include "Html.h"
#include "BetterSockets.h"
#include "Utils.h"
#include "HtmlRequest.h"
#include "HtmlResponse.h"

using namespace html;

void html::sendHTML(int const &socketFD, std::string page, std::string status){
    std::ifstream info;
    HtmlResponse resp(status);

    if(page == "html/"){
        page = HOME;
    }

    info.open(page, std::ios::binary);
    if((info.rdstate() & std::ifstream::failbit) != 0){
        throw std::runtime_error("Failed to open file" + page);
    }

    if(page.find(".ts") != std::string::npos){
        resp.setType(VIDEO_FILE);
    }
    else{
        resp.setType(TEXT_FILE);
    }

    vChar htmlData((std::istreambuf_iterator<char>(info)), std::istreambuf_iterator<char>());
    resp.setContent(htmlData);
    
    bsock::sendToSocket(socketFD, resp.getData());
}

int html::handleHTMLReq(int const &socketFD){
    HtmlRequest req;
    
    try {
        req = bsock::readSocket(socketFD, 1024);
    }
    catch(...){
        return -1;
    }

    switch (utils::hash(req.getMethod().c_str())){
        case utils::hash("GET"):
            try{
                sendHTML(socketFD, req.getUrl(), APPROVED);
            }
            catch(...){
                sendHTML(socketFD, P_NOT_FOUND, NOT_FOUND);
            }
            break;
        
        case utils::hash("POST"):
            break;

        default:
            return -1;
    }

    return 0;
}