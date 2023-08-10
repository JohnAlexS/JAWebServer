#pragma once

#include <string>
#include <sstream>

#define HTML_SRC_PATH "html"

typedef std::string html_comp;

class HtmlRequest{
    public:
        HtmlRequest();
        HtmlRequest(const html_comp& str);
        ~HtmlRequest();
        
        void setMethod(html_comp method);
        html_comp getMethod() const;
        void setUrl(html_comp url);
        html_comp getUrl() const;

        HtmlRequest& operator=(const html_comp& str);

    private:
        html_comp method;
        html_comp url;
};