#pragma once
#ifndef STRINGTEMPLATE_BLOCKREFERENCE_HPP
#define STRINGTEMPLATE_BLOCKREFERENCE_HPP

#include <StringTemplate/Common.hpp>
#include <StringTemplate/String.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

class BlockReference
{
public:

    friend class Template;

    void addText(const String& value);
    void addSymbol(const String& name);
    BlockReference addBlock(const String& name);
    BlockReference block(const String& name);

    void set(const String& name, const String& value);
    bool has(const String& name);
    String get(const String& name);

    void enable();
    void disable();

    BlockReference repeat(unsigned count);
    BlockReference next();

private:

    BlockReference(Block* block);

    BlockReference duplicate();

    Block *block_;

};

STRINGTEMPLATE_NAMESPACE_END

#endif // #ifndef STRINGTEMPLATE_BLOCKREFERENCE_HPP
