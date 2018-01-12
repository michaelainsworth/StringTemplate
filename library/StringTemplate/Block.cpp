#include <boost/format.hpp>
#include <iostream>
#include <StringTemplate/Block.hpp>
#include <StringTemplate/html.hpp>
#include <StringTemplate/Symbol.hpp>
#include <StringTemplate/Text.hpp>

using boost::format;

STRINGTEMPLATE_NAMESPACE_BEGIN

Block::Block()
    : parent_(nullptr)
    , name_()
    , show_(true)
    , dict_()
    , children_()
{}

Block::Block(Block& parent, const String& name)
    : parent_(&parent)
    , name_(name)
    , show_(true)
    , dict_()
    , children_()
{}

Block::~Block()
{
    for (auto child : children_)
    {
        delete child;
    }
    children_.clear();
}

void Block::render(std::ostream& os, bool empty)
{
    if (empty)
    {
        if (show_)
        {
            auto it = children_.begin(), end = children_.end();
            for (; it != end; ++it)
            {
                (*it)->render(os, empty);
            }
        }
    }
    else
    {
        if (name_.size())
        {
            os << "{#" << name_ << "}";
        }
        auto it = children_.begin(), end = children_.end();
        for (; it != end; ++it)
        {
            (*it)->render(os, empty);
        }
        if (name_.size())
        {
            os << "{/" << name_ << "}";
        }
    }
}

Node* Block::clone() const
{
    Block* that = new Block();
    that->parent_ = parent_;
    that->name_ = name_;
    that->show_ = show_;
    that->dict_ = dict_;
    auto it = children_.begin(), end = children_.end();
    for (; it != end; ++it)
    {
        that->children_.push_back((*it)->clone());
    }
    for (auto child : that->children_)
    {
        child->reparent(that);
    }
    return that;
}

void Block::addText(const String& value)
{
    children_.push_back(new Text(value));
}

void Block::addSymbol(const String& name)
{
    children_.push_back(new Symbol(*this, name));
}

Block& Block::addBlock(const String& name)
{
    auto block = new Block(*this, name);
    children_.push_back(block);
    return *block;
}

Block* Block::block(const String& name)
{
    for (auto child : children_)
    {
        auto cblock = dynamic_cast<Block*>(child);
        if (cblock && cblock->name_ == name)
        {
            return cblock;
        }
    }
    return nullptr;
}



Block& Block::duplicate()
{
    if (!parent_)
    {
        throw std::logic_error
        (
            "You can't duplicate the top-level block."
        );
    }

    auto& cn = parent_->children_;
    auto it = std::find(cn.begin(), cn.end(), this);
    if (it == cn.end())
    {
        throw std::logic_error
        (
            "The child block was not located in the parent block."
        );
    }

    Block* other = reinterpret_cast<Block*>(clone());

    ++it;
    if (it == cn.end())
    {
        cn.push_back(other);
    }
    else
    {
        cn.insert(it, other);
    }
    return *other;
}


void Block::reparent(Block* parent)
{
    parent_ = parent;
    for (auto child : children_)
    {
        child->reparent(this);
    }
}

void Block::set(const String& name, const String& value)
{
    auto it = dict_.find(name), end = dict_.end();
    if (it != end)
    {
        dict_.erase(it);
    }

    dict_.insert(std::make_pair(name, html(value)));
}

bool Block::has(const String& name) const
{
    return dict_.find(name) != dict_.end();
}

String Block::get(const String& name) const
{
    auto it = dict_.find(name), end = dict_.end();
    if (it == end)
    {
        throw std::logic_error
        (
            (
                format
                (
                    "The symbol name '%s' has not been set."
                )
                % name
            ).str()
        );
    }

    return it->second;
}

void Block::show()
{
    show_ = true;
}

void Block::hide()
{
    show_ = false;
}

String Block::name() const
{
    return name_;
}

STRINGTEMPLATE_NAMESPACE_END

