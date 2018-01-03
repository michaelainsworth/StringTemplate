#include <boost/format.hpp>
#include <fstream>
#include <iostream>
#include <StringTemplate/Block.hpp>
#include <StringTemplate/Parser.hpp>
#include <StringTemplate/Template.hpp>

using boost::format;

STRINGTEMPLATE_NAMESPACE_BEGIN

Template::Template(const String& filename)
    : block_(nullptr)
{
    try
    {
        String content;
        std::ifstream fin(filename.c_str());

        if (!fin)
        {
            throw std::runtime_error
            (
                (
                    format
                    (
                        "The template file '%s' is not readable."
                    )
                    % filename
                ).str()
            );
        }

        char c;
        while (fin.read(&c, 1))
        {
            content += c;
        }

        Tokenizer tokenizer;
        auto tokens = tokenizer.tokenize(content);

        Parser parser;
        block_.reset(parser.parse(tokens));
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error
        (
            (
                format
                (
                    "An error occurred when loading the template. %s"
                )
                % e.what()
            ).str()
        );
    }
}

Template::~Template()
{
}

void Template::render(std::ostream& os)
{
    block_->render(os);
}

void Template::addText(const String& value)
{
    block_->addText(value);
}

void Template::addSymbol(const String& name)
{
    block_->addSymbol(name);
}

BlockReference Template::addBlock(const String& name)
{
    auto& other = block_->addBlock(name);
    return BlockReference(&other);
}

BlockReference Template::block(const String& name)
{
    Block* child = block_->block(name);
    return BlockReference(child);
}

void Template::set(const String& name, const String& value)
{
    block_->set(name, value);
}

bool Template::has(const String& name) const
{
    return block_->has(name);
}

String Template::get(const String& name) const
{
    return block_->get(name);
}


STRINGTEMPLATE_NAMESPACE_END

