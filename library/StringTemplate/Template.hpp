#pragma once
#ifndef STRINGTEMPLATE_TEMPLATE_HPP
#define STRINGTEMPLATE_TEMPLATE_HPP

#include <iosfwd>
#include <memory>
#include <StringTemplate/BlockReference.hpp>
#include <StringTemplate/Common.hpp>
#include <StringTemplate/String.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

class Template
{
public:

    Template();
    ~Template();

    void render(std::ostream& os);

    void addText(const String& value);
    void addSymbol(const String& name);
    BlockReference addBlock(const String& name);
    BlockReference block(const String& name);

    void set(const String& name, const String& value);
    bool has(const String& name) const;
    String get(const String& name) const;

private:

    std::shared_ptr<Block> block_;

};

STRINGTEMPLATE_NAMESPACE_END

#endif // #ifndef STRINGTEMPLATE_TEMPLATE_HPP

