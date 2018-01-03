#pragma once
#ifndef STRINGTEMPLATE_TEXT_HPP
#define STRINGTEMPLATE_TEXT_HPP

#include <StringTemplate/Node.hpp>
#include <StringTemplate/String.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

class Text : public Node
{
public:

    Text(const String& value);
    virtual void render(std::ostream& os);
    virtual void reparent(Block* parent);
    virtual Node* clone() const;

private:

    String value_;

};

STRINGTEMPLATE_NAMESPACE_END

#endif // #ifndef STRINGTEMPLATE_TEXT_HPP
