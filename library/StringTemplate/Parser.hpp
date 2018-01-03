#pragma once
#ifndef STRINGTEMPLATE_PARSER_HPP
#define STRINGTEMPLATE_PARSER_HPP

#include <StringTemplate/Tokenizer.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

class Block;

class Parser
{
public:

    Block* parse(const TokenVector& tokens);

};

STRINGTEMPLATE_NAMESPACE_END

#endif // #ifndef STRINGTEMPLATE_PARSER_HPP
