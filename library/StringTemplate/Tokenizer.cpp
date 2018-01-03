#include <boost/format.hpp>
#include <iostream>
#include <stdexcept>
#include <StringTemplate/Tokenizer.hpp>

using boost::format;

STRINGTEMPLATE_NAMESPACE_BEGIN

TokenVector Tokenizer::tokenize(const String& content)
{
    try
    {
        TokenVector tokens;
        String::size_type i = 0, s = content.size();
        String current;

        // 0 == text
        // 1 == open block
        // 2 == close block
        // 3 == symbol
        int mode = 0;

        auto addTextIfNotEmpty = [&tokens, &current, &i, &mode](int increment, int
                                 newMode)
        {
            if (current.size())
            {
                tokens.push_back(Token(TextType, current));
                current.clear();
            }

            i += increment;
            mode = newMode;
        };

        auto addOther = [&content, &tokens, &current, &i, &mode, s](TokenType type)
        {
            char c;
            bool found = false;
            while (i < s)
            {
                c = content[i];

                if ('}' == c && i < s - 1 && '}' == content[i + 1])
                {
                    found = true;
                    ++i;
                    break;
                }
                else
                {
                    current += c;
                }

                ++i;
            }

            if (!found)
            {
                String error;
                if (OpenBlockType == type)
                {
                    error = "An opening {{#block}} was unterminated.";
                }
                else if (CloseBlockType == type)
                {
                    error = "A closing {{/block}} was unterminated.";
                }
                else
                {
                    error = "A {{symbol}} was untermindated.";
                }

                throw std::runtime_error(error);
            }

            tokens.push_back(Token(type, current));
            current.clear();
            mode = 0;
        };

        while (i < s)
        {
            char c = content[i];

            if (0 == mode)
            {
                if ('{' == c)
                {
                    // {{#
                    if (i < s - 2 && content[1 + i] == '{' && content[2 + i] == '#')
                    {
                        addTextIfNotEmpty(2, 1);
                    }
                    // {{/
                    else if (i < s - 2 && content[1 + i] == '{' && content[2 + i] == '/')
                    {
                        addTextIfNotEmpty(2, 2);
                    }
                    // {{a
                    else if (i < s - 2 && content[1 + i] == '{')
                    {
                        addTextIfNotEmpty(1, 3);
                    }
                    else
                    {
                        current += c;
                    }
                }
                else
                {
                    current += c;
                }
            }
            else if (1 == mode)
            {
                addOther(OpenBlockType);
            }
            else if (2 == mode)
            {
                addOther(CloseBlockType);
            }
            else if (3 == mode)
            {
                addOther(SymbolType);
            }

            ++i;
        }

        return tokens;
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error
        (
            (
                format
                (
                    "An error occurred when tokenizing the template. %s"
                )
                % e.what()
            ).str()
        );
    }
}


STRINGTEMPLATE_NAMESPACE_END

