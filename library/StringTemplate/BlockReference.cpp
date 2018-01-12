#include <StringTemplate/BlockReference.hpp>
#include <algorithm>
#include <StringTemplate/Block.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

BlockReference::BlockReference(Block* block)
    : block_(block)
{}

BlockReference BlockReference::addText(const String& value)
{
    if (block_)
    {
        block_->addText(value);
    }

    return *this;
}

BlockReference BlockReference::addSymbol(const String& name)
{
    if (block_)
    {
        block_->addSymbol(name);
    }

    return *this;
}

BlockReference BlockReference::addBlock(const String& name)
{
    if (block_)
    {
        auto& other = block_->addBlock(name);
        return BlockReference(&other);
    }

    return BlockReference(nullptr);
}

BlockReference BlockReference::block(const String& name)
{
    if (block_)
    {
        auto other = block_->block(name);
        return BlockReference(other);
    }

    return BlockReference(nullptr);
}

BlockReference BlockReference::set(const String& name, const String& value)
{
    if (block_)
    {
        block_->set(name, value);
    }

    return *this;
}

bool BlockReference::has(const String& name)
{
    if (block_)
    {
        return block_->has(name);
    }

    return false;
}

String BlockReference::get(const String& name)
{
    if (block_)
    {
        return block_->get(name);
    }

    return "";
}

BlockReference BlockReference::enable()
{
    if (block_)
    {
        block_->enable();
    }

    return *this;
}

BlockReference BlockReference::disable()
{
    if (block_)
    {
        block_->disable();
    }

    return *this;
}

BlockReference BlockReference::duplicate()
{
    if (block_)
    {
        auto& other = block_->duplicate();
        return BlockReference(&other);
    }

    return BlockReference(nullptr);
}

BlockReference BlockReference::repeat(unsigned count)
{
    if (block_)
    {
        for (unsigned i = 1; i < count; ++i)
        {
            duplicate();
        }
    }

    return *this;
}

BlockReference BlockReference::next()
{
    if (!block_)
    {
        return BlockReference(nullptr);
    }

    auto& pc = block_->parent_->children_;
    auto begin = pc.begin(), end = pc.end();
    auto it = std::find(begin, end, block_);
    if (it == end)
    {
        return BlockReference(nullptr);
    }

    ++it;
    for (; it != end; ++it)
    {
        auto test = dynamic_cast<Block*>(*it);
        if (test && test->name_ == block_->name_)
        {
            return BlockReference(test);
        }
    }

    return BlockReference(nullptr);
}

STRINGTEMPLATE_NAMESPACE_END

