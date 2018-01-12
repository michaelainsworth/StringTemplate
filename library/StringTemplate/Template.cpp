#include <boost/format.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <StringTemplate/Block.hpp>
#include <StringTemplate/File.hpp>
#include <StringTemplate/Parser.hpp>
#include <StringTemplate/Template.hpp>

using boost::format;

STRINGTEMPLATE_NAMESPACE_BEGIN

Template::Template(const String& filename)
    : block_(nullptr)
{
    try
    {
        auto content = getFileContents(filename);
        std::istringstream buffer(content);
        parse(buffer);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error
        (
            (
                format
                (
                    "Error in '%s': %s"
                )
                % filename
                % e.what()
            ).str()
        );
    }
}

Template::Template(std::istream& buffer)
    : block_(nullptr)
{
    try
    {
        parse(buffer);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error
        (
            (
                format
                (
                    "Error in buffer: %s"
                )
                % e.what()
            ).str()
        );
    }
}

Template::~Template()
{
}

void Template::render(std::ostream& os, bool empty)
{
    block_->render(os, empty);
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

Template& Template::set(const String& name, const String& value)
{
    block_->set(name, value);
    return *this;
}

Template& Template::set(const String& name, const char* value)
{
    return set(name, String(value ? value : ""));
}

bool Template::has(const String& name) const
{
    return block_->has(name);
}

String Template::get(const String& name) const
{
    return block_->get(name);
}

void Template::parse(std::istream& ins)
{
    String content;
    char c;
    while (ins.get(c))
    {
        content += c;
    }

    Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize(content);

    Parser parser;
    block_.reset(parser.parse(tokens));
}

STRINGTEMPLATE_NAMESPACE_END

