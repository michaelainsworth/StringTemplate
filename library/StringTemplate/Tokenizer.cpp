#include <boost/format.hpp>
#include <iostream>
#include <map>
#include <stdexcept>
#include <StringTemplate/Tokenizer.hpp>

using boost::format;

STRINGTEMPLATE_NAMESPACE_BEGIN

TokenVector Tokenizer::tokenize(const String& content)
{
    try
    {
        content_ = &content;
        i_ = 0;
        s_ = content_->size();
        current_.clear();
        mode_.clear();
        mode_.push_back(TokenizeText);
        tokens_.clear();

        doTokenize();

        return tokens_;
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error
        (
            (
                format
                (
                    "Token error: %s"
                )
                % e.what()
            ).str()
        );
    }
}

bool Tokenizer::isIdentifier(char c)
{
    return ('_' == c)             ||
           (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}

bool Tokenizer::isFilename(char c)
{
    return ('_' == c)             ||
           ('-' == c)             ||
           ('_' == c)             ||
           ('.' == c)             ||
           (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}

void Tokenizer::dump(const TokenVector& tokens)
{
    for (auto token : tokens)
    {
        std::cout << token.description() << std::endl;
    }
}

void Tokenizer::dumpState()
{
    std::cout << "Mode Stack\n";
    for (auto m : mode_)
    {
        std::cout << "    " << m << std::endl;
    }
}

void Tokenizer::doTokenize()
{
    typedef void (Tokenizer::*Callback)();
    std::map<Mode,Callback> processor;
    processor[TokenizeText]             = &Tokenizer::tokenizeText;
    processor[TokenizeOpenBlock]        = &Tokenizer::tokenizeOpenBlock;
    processor[TokenizeCloseBlock]       = &Tokenizer::tokenizeCloseBlock;
    processor[TokenizeSymbol]           = &Tokenizer::tokenizeSymbol;
    processor[TokenizeInclude]          = &Tokenizer::tokenizeInclude;
    processor[TokenizeIncludeArguments] = &Tokenizer::tokenizeIncludeArguments;

    while (mode_.size() && i_ < s_)
    {
        auto m = mode_.back();
        auto it = processor.find(m), end = processor.end();
        if (it == end && i_ < s_)
        {
            throw std::logic_error
            (
                "Unknown processing mode."
            );
        }

        auto callback = it->second;
        ((*this).*(callback))();
    }

    if (!mode_.size() || mode_[0] != TokenizeText)
    {
        throw std::runtime_error
        (
            "Corrupt token mode stack."
        );
    }
}

void Tokenizer::tokenizeSimple(TokenType type)
{
    bool found = false;
    while (i_ < s_)
    {
        char c = (*content_)[i_];

        if (isIdentifier(c))
        {
            current_ += c;
        }
        else if ('}' == c)
        {
            // Remove newline after {#block} and {/block} elements.
            if (isEndWithNewline(type))
            {
                ++i_;
            }

            found = true;
            ++i_;

            break;
        }
        else
        {
            throw std::runtime_error
            (
                (
                    format
                    (
                        "Bad identifier at %i."
                    )
                    % (i_ + 1)
                ).str()
            );
        }

        ++i_;
    }

    if (!found)
    {
        String error;
        if (OpenBlockType == type)
        {
            error = "Unterminated {#block} at %i.";
        }
        else if (CloseBlockType == type)
        {
            error = "Unterminated {/block} at %i.";
        }
        else
        {
            error = "Unterminated {$symbol} at %i.";
        }

        throw std::runtime_error
        (
            (
                format
                (
                    error
                )
                % (i_ + 1)
            ).str()
        );
    }

    tokens_.push_back(Token(type, current_));
    current_.clear();
    mode_.pop_back();
}

void Tokenizer::tokenizeText()
{
    while (i_ < s_)
    {
        char c = (*content_)[i_];

        if ('{' == c)
        {
            // {#
            if (i_ < s_ - 1 && (*content_)[1 + i_] == '#')
            {
                mode_.push_back(TokenizeOpenBlock);
                i_ += 2;
                break;
            }
            // {/
            else if (i_ < s_ - 1 && (*content_)[1 + i_] == '/')
            {
                mode_.push_back(TokenizeCloseBlock);
                i_ += 2;
                break;
            }
            // {$
            else if (i_ < s_ - 1 && (*content_)[1 + i_] == '$')
            {
                mode_.push_back(TokenizeSymbol);
                i_ += 2;
                break;
            }
            // {=
            else if (i_ < s_ - 1 && (*content_)[1 + i_] == '=')
            {
                mode_.push_back(TokenizeInclude);
                i_ += 2;
                break;
            }
            else
            {
                current_ += c;
            }
        }
        else
        {
            current_ += c;
        }

        ++i_;
    }

    if (current_.size())
    {
        tokens_.push_back(Token(TextType, current_));
        current_.clear();
    }
}

void Tokenizer::tokenizeOpenBlock()
{
    tokenizeSimple(OpenBlockType);
}

void Tokenizer::tokenizeCloseBlock()
{
    tokenizeSimple(CloseBlockType);
}

void Tokenizer::tokenizeSymbol()
{
    tokenizeSimple(SymbolType);
}

void Tokenizer::tokenizeInclude()
{
    while (i_ < s_)
    {
        char c = (*content_)[i_];

        if (isFilename(c))
        {
            current_ += c;
        }
        else if ('}' == c || isSpace(c))
        {
            mode_.pop_back();
            mode_.push_back(TokenizeIncludeArguments);
            break;
        }
        else
        {
            throw std::runtime_error
            (
                (
                    format
                    (
                        "Bad {=filename} at %i."
                    )
                    % (i_ + 1)
                ).str()
            );
        }

        ++i_;
    }

    if (!current_.size())
    {
        throw std::runtime_error
        (
            "Empty {=include}."
        );
    }

    tokens_.push_back(Token(IncludeType, current_));
    current_.clear();
}

void Tokenizer::tokenizeIncludeArguments()
{
    bool found = false;

    while (i_ < s_)
    {
        char c = (*content_)[i_];

        if ('\'' == c || '"' == c)
        {
            ++i_;
            tokenizeString(c);
        }
        else if (isSpace(c))
        {
            // Skip
        }
        else if ('}' == c)
        {
            if (isEndWithNewline(IncludeType))
            {
                ++i_;
            }

            mode_.pop_back();
            ++i_;
            found = true;

            break;
        }
        else
        {
            throw std::runtime_error
            (
                (
                    format
                    (
                        "Unknown character '%c' at %i."
                    )
                    % c
                    % (i_ + 1)
                ).str()
            );
        }

        ++i_;
    }

    if (!found)
    {
        throw std::runtime_error
        (
            "Unterminated {=include}."
        );
    }
}

bool Tokenizer::isEndWithNewline(TokenType type)
{
    if (type == SymbolType)
    {
        return false;
    }

    if (i_ >= s_ - 1)
    {
        return false;
    }

    char c = (*content_)[i_ + 1];

    return ('\r' == c || '\n' == c);
}

bool Tokenizer::isSpace(char c)
{
    return '\r' == c ||
           '\n' == c ||
           '\t' == c ||
           ' '  == c;
}

void Tokenizer::tokenizeString(char delimiter)
{
    auto start = i_;
    bool found = false;
    while (i_ < s_)
    {
        char c = (*content_)[i_];

        if (delimiter == c)
        {
            tokens_.push_back(Token(StringType, current_));
            current_.clear();
            found = true;
            break;
        }
        else if ('\\' == c)
        {
            if (i_ < s_ - 1)
            {
                char n = (*content_)[i_ + 1];
                if (n == delimiter)
                {
                    current_ += delimiter;
                    ++i_;
                }
                else if (n == '\\')
                {
                    current_ += n;
                    ++i_;
                }
                else if (n == '\r')
                {
                    current_ += "\r";
                    ++i_;
                }
                else if (n == '\n')
                {
                    current_ += "\n";
                    ++i_;
                }
                else if (n == '\t')
                {
                    current_ += "\t";
                    ++i_;
                }
                else
                {
                    current_ += c;
                }
            }
            else
            {
                current_ += c;
            }
        }
        else
        {
            current_ += c;
        }

        ++i_;
    }

    if (!found)
    {
        throw std::runtime_error
        (
            (
                format
                (

                    "Unterminated string at %i "
                )
                % start
            ).str()
        );
    }
}

STRINGTEMPLATE_NAMESPACE_END

