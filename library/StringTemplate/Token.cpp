#include <StringTemplate/Token.hpp>
#include <cctype>

STRINGTEMPLATE_NAMESPACE_BEGIN

Token::Token(TokenType type, const String& value)
    : type_(type)
    , value_(value)
{}

TokenType Token::type() const
{
    return type_;
}

String Token::description() const
{
    String t;
    if (TextType == type_) t = "Text";
    if (OpenBlockType == type_) t = "OpenBlock";
    if (CloseBlockType == type_) t = "CloseBlock";
    if (SymbolType == type_) t = "Symbol";
    if (IncludeType == type_) t = "Include";
    if (StringType == type_) t = "String";

    t += ": ";
    if (TextType == type_ || StringType == type_)
    {
        for (auto c : value_)
        {
            if (isTextLike(c))
            {
                t += c;
            }
            else
            {
                t += ' ';
            }
        }
    }
    else
    {
        t += value_;
    }

    return t;
}

String Token::value() const
{
    return value_;
}

bool Token::isTextLike(char c)
{
    return ('~' == c)             ||
           ('`' == c)             ||
           ('!' == c)             ||
           ('@' == c)             ||
           ('#' == c)             ||
           ('$' == c)             ||
           ('%' == c)             ||
           ('^' == c)             ||
           ('&' == c)             ||
           ('*' == c)             ||
           ('(' == c)             ||
           (')' == c)             ||
           ('_' == c)             ||
           ('-' == c)             ||
           ('+' == c)             ||
           ('=' == c)             ||
           ('[' == c)             ||
           ('{' == c)             ||
           (']' == c)             ||
           ('}' == c)             ||
           ('\\' == c)            ||
           ('|' == c)             ||
           (';' == c)             ||
           (':' == c)             ||
           ('\'' == c)            ||
           ('"' == c)             ||
           ('<' == c)             ||
           (',' == c)             ||
           ('>' == c)             ||
           ('.' == c)             ||
           ('/' == c)             ||
           ('?' == c)             ||
           (' ' == c)             ||
           (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}

STRINGTEMPLATE_NAMESPACE_END

