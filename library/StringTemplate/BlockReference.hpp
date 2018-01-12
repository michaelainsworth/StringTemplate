#pragma once
#ifndef STRINGTEMPLATE_BLOCKREFERENCE_HPP
#define STRINGTEMPLATE_BLOCKREFERENCE_HPP

#include <boost/lexical_cast.hpp>
#include <StringTemplate/Common.hpp>
#include <StringTemplate/String.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

class BlockReference
{
public:

    friend class Template;

    BlockReference addText(const String& value);
    BlockReference addSymbol(const String& name);
    BlockReference addBlock(const String& name);
    BlockReference block(const String& name);

    BlockReference set(const String& name, const String& value);
    BlockReference set(const String& name, const char* value);
    template<typename T>
    BlockReference set(const String& name, const T& value);

    bool has(const String& name);
    String get(const String& name);

    BlockReference show();
    BlockReference hide();

    BlockReference repeat(unsigned count);
    BlockReference next();

private:

    BlockReference(Block* block);

    BlockReference duplicate();

    Block *block_;

};

template<typename T>
inline BlockReference BlockReference::set(const String& name, const T& value)
{
    return set(name, boost::lexical_cast<String>(value));
}

STRINGTEMPLATE_NAMESPACE_END

#endif // #ifndef STRINGTEMPLATE_BLOCKREFERENCE_HPP
