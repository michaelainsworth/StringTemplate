#pragma once
#ifndef STRINGTEMPLATE_SYMBOL_HPP
#define STRINGTEMPLATE_SYMBOL_HPP

#include <StringTemplate/Node.hpp>
#include <StringTemplate/String.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

class Symbol : public Node
{
public:

    Symbol(Block& parent, const String& name);
    virtual Node* clone() const;
    virtual void reparent(Block* parent);
    virtual void render(std::ostream& os, bool empty);

private:

    Block* parent_;
    String name_;

};

STRINGTEMPLATE_NAMESPACE_END

#endif // #ifndef STRINGTEMPLATE_SYMBOL_HPP
