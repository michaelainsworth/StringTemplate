#include <iostream>
#include <StringTemplate/Block.hpp>
#include <StringTemplate/Template.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

Template::Template()
    : block_(new Block())
{}

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

