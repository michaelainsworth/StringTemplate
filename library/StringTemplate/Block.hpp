#pragma once
#ifndef STRINGTEMPLATE_BLOCK_HPP
#define STRINGTEMPLATE_BLOCK_HPP

#include <StringTemplate/Node.hpp>
#include <StringTemplate/String.hpp>
#include <vector>
#include <map>

STRINGTEMPLATE_NAMESPACE_BEGIN

class Block : public Node
{
private:

    typedef Node* NodePointer;
    typedef std::vector<NodePointer> NodePointerVector;
    typedef std::map<String,String> Dictionary;

public:

    friend class BlockReference;
    friend class Parser;

    Block();
    virtual Node* clone() const;
    void reparent(Block* parent);
    Block(Block& parent, const String& name);
    ~Block();

    Block& duplicate();

    void render(std::ostream& os, bool empty);

    void addText(const String& value);
    void addSymbol(const String& name);
    Block& addBlock(const String& name);
    Block* block(const String& name);

    void set(const String& name, const String& value);
    bool has(const String& name) const;
    String get(const String& name) const;

    void enable();
    void disable();

    String name() const;

private:

    Block* parent_;
    String name_;
    bool enabled_;
    Dictionary dict_;
    NodePointerVector children_;

};

STRINGTEMPLATE_NAMESPACE_END

#endif // #ifndef STRINGTEMPLATE_BLOCK_HPP
