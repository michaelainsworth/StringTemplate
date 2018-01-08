#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <sstream>
#include <StringTemplate/Block.hpp>
#include <StringTemplate/File.hpp>
#include <StringTemplate/Parser.hpp>
#include <StringTemplate/Template.hpp>

using boost::format;

STRINGTEMPLATE_NAMESPACE_BEGIN

Parser::Parser()
    : root_(nullptr)
    , context_()
    , tokens_(nullptr)
    , i_(0)
    , s_(0)
{}

Parser::~Parser()
{
    if (root_)
    {
        delete root_;
        root_ = nullptr;
    }

    context_.clear();
}

Block* Parser::parse(const TokenVector& tokens)
{
    try
    {
        if (root_)
        {
            delete root_;
        }

        root_ = new Block();
        context_.clear();
        context_.push_back(root_);
        current_ = root_;
        tokens_ = &tokens;
        i_ = 0;
        s_ = tokens_->size();

        return doParse();
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error
        (
            (
                format
                (
                    "Parse error: %s"
                )
                % e.what()
            ).str()
        );
    }
}

Block* Parser::doParse()
{
    while (i_ < s_)
    {
        auto token = (*tokens_)[i_];
        auto type = token.type();
        auto value = token.value();

        if (TextType == type)
        {
            currentBlock().addText(value);
        }
        else if (OpenBlockType == type)
        {
            auto& child = currentBlock().addBlock(value);
            context_.push_back(&child);
            current_ = &child;
        }
        else if (CloseBlockType == type)
        {
            if (1 == context_.size())
            {
                throw std::runtime_error
                (
                    (
                        format
                        (
                            "Unexpected {/block}."
                        )
                        % value
                    ).str()
                );
            }
            else
            {
                if (value != currentBlock().name())
                {
                    throw std::runtime_error
                    (
                        (
                            format
                            (
                                "Mismatched {#%s} and {/%s}."
                            )
                            % currentBlock().name()
                            % value
                        ).str()
                    );
                }
                else
                {
                    context_.pop_back();
                    current_ = context_[context_.size() - 1];
                }
            }
        }
        else if (SymbolType == type)
        {
            currentBlock().addSymbol(value);
        }
        else if (IncludeType == type)
        {
            Block* block = nullptr;
            StringVector arguments;

            ++i_;
            while (i_ < s_)
            {
                auto next = (*tokens_)[i_];
                if (next.type() == StringType)
                {
                    arguments.push_back(next.value());
                }
                else
                {
                    --i_;
                    break;
                }

                ++i_;
            }

            try
            {
                Tokenizer tokenizer;
                Parser parser;

                auto tokens = tokenizer.tokenize(getFileContents(value));
                block = parser.parse(tokens);

                StringVector::size_type x = 0, k = arguments.size();
                while (x < k)
                {
                    auto xs = boost::lexical_cast<String>(x + 1);
                    block->set(xs, arguments[x]);

                    ++x;
                }

                std::stringstream buffer;
                block->render(buffer, false);
                delete block;
                block = nullptr;

                tokens = tokenizer.tokenize(buffer.str());
                block = parser.parse(tokens);

                for (auto bchild : block->children_)
                {
                    currentBlock().children_.push_back(bchild);
                    bchild->reparent(&currentBlock());
                }
                block->children_.clear();
                delete block;
            }
            catch (const std::exception& e)
            {
                if (block)
                {
                    delete block;
                }

                throw std::runtime_error
                (
                    (
                        format
                        (
                            "Error in {=%s}: %s"
                        )
                        % value
                        % e.what()
                    ).str()
                );
            }
        }

        ++i_;
    }

    auto ret = root_;
    root_ = nullptr;
    return ret;
}

Block& Parser::currentBlock()
{
    auto block = dynamic_cast<Block*>(current_);
    if (!block)
    {
        throw std::logic_error
        (
            "Current node is not a block."
        );
    }
    return *block;
}

STRINGTEMPLATE_NAMESPACE_END

