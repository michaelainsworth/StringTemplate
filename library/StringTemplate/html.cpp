#include "html.hpp"

STRINGTEMPLATE_NAMESPACE_BEGIN

String html(const std::string& raw)
{
    std::string result;

    for (auto c : raw)
    {
        if ('&' == c)
        {
            result += "&amp;";
        }
        else if ('<' == c)
        {
            result += "&lt;";
        }
        else if ('>' == c)
        {
            result += "&gt;";
        }
        else if ('"' == c)
        {
            result += "&quot;";
        }
        else if ('\'' == c)
        {
            result += "&#39;";
        }
        else
        {
            result += c;
        }
    }

    return result;
}

STRINGTEMPLATE_NAMESPACE_END

