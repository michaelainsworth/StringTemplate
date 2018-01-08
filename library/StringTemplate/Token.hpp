#pragma once
#ifndef STRINGTEMPLATE_TOKEN_HPP
#define STRINGTEMPLATE_TOKEN_HPP

#include <StringTemplate/Common.hpp>
#include <StringTemplate/String.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

enum TokenType
{
    TextType,
    SymbolType,
    OpenBlockType,
    CloseBlockType,
    IncludeType,
    StringType
};

class Token
{
public:

    Token(TokenType type, const String& value);

    TokenType type() const;
    String description() const;
    String value() const;
    
private:

    static bool isTextLike(char c);

    TokenType type_;
    String value_;

};

STRINGTEMPLATE_NAMESPACE_END

#endif // #ifndef STRINGTEMPLATE_TOKEN_HPP

