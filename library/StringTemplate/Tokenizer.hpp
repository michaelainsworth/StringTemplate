#pragma once
#ifndef STRINGTEMPLATE_TOKENIZER_HPP
#define STRINGTEMPLATE_TOKENIZER_HPP

#include <StringTemplate/Common.hpp>
#include <StringTemplate/Token.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

typedef std::vector<Token> TokenVector;

class Tokenizer
{
public:

    TokenVector tokenize(const String& content);

    static bool isIdentifier(char c);

};

STRINGTEMPLATE_NAMESPACE_END

#endif // #ifndef STRINGTEMPLATE_TOKENIZER_HPP

