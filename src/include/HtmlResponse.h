#include <string>
#include <vector>

typedef std::string html_comp;
typedef std::vector<char> vChar;

class HtmlResponse{
    public:
        HtmlResponse();
        HtmlResponse(html_comp status, int length, html_comp content, html_comp type);
        HtmlResponse(html_comp status);
        ~HtmlResponse();

        void setStatus(html_comp status);
        void setType(html_comp type);
        void setContent(vChar &content);
        void setContent(html_comp content);

        html_comp getData() const;

    private:
        html_comp status;
        int length;
        html_comp content_type;
        html_comp content;
};