#include <iostream>
#include <StringTemplate/Text.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

Text::Text(const String& value)
    : value_(value)
{}

void Text::render(std::ostream& os)
{
    os.write(value_.c_str(), value_.size());
}

void Text::reparent(Block* parent)
{
    // Nothing.
}

Node* Text::clone() const
{
    return new Text(value_);
}

STRINGTEMPLATE_NAMESPACE_END

