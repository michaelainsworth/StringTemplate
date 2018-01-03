#pragma once
#ifndef STRINGTEMPLATE_NODE_HPP
#define STRINGTEMPLATE_NODE_HPP

#include <boost/noncopyable.hpp>
#include <iosfwd>
#include <StringTemplate/Common.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

class Node : private boost::noncopyable
{
public:

    virtual ~Node() = 0;
    virtual Node* clone() const = 0;
    virtual void reparent(Block* parent) = 0;
    virtual void render(std::ostream& os) = 0;

};

STRINGTEMPLATE_NAMESPACE_END

#endif // #ifndef STRINGTEMPLATE_NODE_HPP
