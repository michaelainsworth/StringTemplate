#include <iostream>
#include <StringTemplate/Block.hpp>
#include <StringTemplate/Symbol.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

Symbol::Symbol(Block& parent, const String& name)
    : parent_(&parent)
    , name_(name)
{}

void Symbol::render(std::ostream& os)
{
    if (!parent_->has(name_))
    {
        os << "{{" << name_ << "}}";
    }
    else
    {
        auto value = parent_->get(name_);
        os.write(value.c_str(), value.size());
    }
}

void Symbol::reparent(Block* parent)
{
    parent_ = parent;
}

Node* Symbol::clone() const
{
    return new Symbol(*parent_, name_);
}

STRINGTEMPLATE_NAMESPACE_END

