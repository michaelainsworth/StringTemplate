#pragma once
#ifndef STRINGTEMPLATE_PARSER_HPP
#define STRINGTEMPLATE_PARSER_HPP

#include <StringTemplate/Tokenizer.hpp>
#include <boost/noncopyable.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

class Node;
class Block;

class Parser : private boost::noncopyable
{
public:

    Parser();
    ~Parser();

    Block* parse(const TokenVector& tokens);

private:

    Block* doParse();
    
    typedef std::vector<Node*> Context;

    Block& currentBlock();

    Block* root_;
    Context context_;
    Node* current_;
    const TokenVector* tokens_;
    TokenVector::size_type i_;
    TokenVector::size_type s_;

};

STRINGTEMPLATE_NAMESPACE_END

#endif // #ifndef STRINGTEMPLATE_PARSER_HPP
