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

    t += ": ";
    if (SymbolType == type_)
    {
        for (auto c : value_)
        {
            if (std::isprint(c))
            {
                t += c;
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

STRINGTEMPLATE_NAMESPACE_END

