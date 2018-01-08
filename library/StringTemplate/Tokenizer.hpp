#pragma once
#ifndef STRINGTEMPLATE_TOKENIZER_HPP
#define STRINGTEMPLATE_TOKENIZER_HPP

#include <boost/noncopyable.hpp>
#include <StringTemplate/Common.hpp>
#include <StringTemplate/Token.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

typedef std::vector<Token> TokenVector;

class Tokenizer : private boost::noncopyable
{
public:

    TokenVector tokenize(const String& content);

    static void dump(const TokenVector& tokens);
    void dumpState();

private:

    enum Mode
    {
        TokenizeText,
        TokenizeOpenBlock,
        TokenizeCloseBlock,
        TokenizeSymbol,
        TokenizeInclude,
        TokenizeIncludeArguments
    };

    void doTokenize();

    static bool isIdentifier(char c);
    static bool isSpace(char c);
    static bool isFilename(char c);

    void tokenizeSimple(TokenType type);
    String tokenizeString();


    void tokenizeText();
    void tokenizeOpenBlock();
    void tokenizeCloseBlock();
    void tokenizeSymbol();
    void tokenizeInclude();
    void tokenizeIncludeArguments();
    void tokenizeString(char delimiter);
    bool isEndWithNewline(TokenType type);

    typedef std::vector<Mode> ModeVector;

    const String* content_;
    String::size_type i_;
    String::size_type s_;
    String current_;
    ModeVector mode_;
    TokenVector tokens_;


};

STRINGTEMPLATE_NAMESPACE_END

#endif // #ifndef STRINGTEMPLATE_TOKENIZER_HPP

