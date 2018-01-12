#pragma once
#ifndef STRINGTEMPLATE_TEMPLATE_HPP
#define STRINGTEMPLATE_TEMPLATE_HPP

#include <boost/lexical_cast.hpp>
#include <iosfwd>
#include <memory>
#include <StringTemplate/BlockReference.hpp>
#include <StringTemplate/Common.hpp>
#include <StringTemplate/String.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

class Template
{
public:

    friend class Parser;

    Template(const String& filename);
    Template(std::istream& buffer);
    ~Template();

    void render(std::ostream& os, bool empty = true);

    void addText(const String& value);
    void addSymbol(const String& name);
    BlockReference addBlock(const String& name);
    BlockReference block(const String& name);

    Template& set(const String& name, const String& value);
    Template& set(const String& name, const char* value);

    template<typename T>
    Template& set(const String& name, const T& value);


    bool has(const String& name) const;
    String get(const String& name) const;

private:

    void parse(std::istream& ins);

    std::shared_ptr<Block> block_;

};

template<typename T>
inline Template& Template::set(const String& name, const T& value)
{
    return set(name, boost::lexical_cast<String>(value));
}

STRINGTEMPLATE_NAMESPACE_END

#endif // #ifndef STRINGTEMPLATE_TEMPLATE_HPP

