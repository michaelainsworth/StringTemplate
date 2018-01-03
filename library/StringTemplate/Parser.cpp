#include <boost/format.hpp>
#include <StringTemplate/Block.hpp>
#include <StringTemplate/Parser.hpp>

using boost::format;

STRINGTEMPLATE_NAMESPACE_BEGIN

Block* Parser::parse(const TokenVector& tokens)
{
    Block* root = new Block();
    std::vector<Block*> context;
    context.push_back(root);
    Block* current = root;

    try
    {
        TokenVector::size_type i = 0, s = tokens.size();

        while (i < s)
        {
            auto token = tokens[i];
            auto type = token.type();
            auto value = token.value();

            if (TextType == type)
            {
                current->addText(value);
            }
            else if (OpenBlockType == type)
            {
                auto& child = current->addBlock(value);
                context.push_back(&child);
                current = &child;
            }
            else if (CloseBlockType == type)
            {
                if (1 == context.size())
                {
                    throw std::runtime_error
                    (
                        (
                            format
                            (
                                "The root block cannot be closed by a {{/%s}}."
                            )
                            % value
                        ).str()
                    );
                }
                else
                {
                    if (value != current->name())
                    {
                        throw std::runtime_error
                        (
                            (
                                format
                                (
                                    "The block {{#%s}} cannot be closed by a {{/%s}}."
                                )
                                % current->name()
                                % value
                            ).str()
                        );
                    }
                    else
                    {
                        context.pop_back();
                        current = context[context.size() - 1];
                    }
                    
                }
            }
            else if (SymbolType == type)
            {
                current->addSymbol(value);
            }

            ++i;
        }

        return root;
    }
    catch (const std::exception& e)
    {
        delete root;
        throw std::runtime_error
        (
            (
                format
                (
                    "An error occurred when parsing the template. %s"
                )
                % e.what()
            ).str()
        );
    }
}

STRINGTEMPLATE_NAMESPACE_END

