#include <boost/format.hpp>
#include <fstream>
#include <StringTemplate/File.hpp>

using boost::format;

STRINGTEMPLATE_NAMESPACE_BEGIN

String getFileContents(const String& filename)
{
    String content;
    std::ifstream fin(filename.c_str());

    if (!fin)
    {
        throw std::runtime_error
        (
            (
                format
                (
                    "Unreadable file '%s':"
                )
                % filename
            ).str()
        );
    }

    char c;
    while (fin.read(&c, 1))
    {
        content += c;
    }

    return content;
}

STRINGTEMPLATE_NAMESPACE_END


