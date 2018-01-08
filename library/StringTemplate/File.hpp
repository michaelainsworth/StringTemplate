#pragma once
#ifndef STRINGTEMPLATE_FILE_HPP
#define STRINGTEMPLATE_FILE_HPP

#include <StringTemplate/Common.hpp>
#include <StringTemplate/String.hpp>

STRINGTEMPLATE_NAMESPACE_BEGIN

String getFileContents(const String& filename);

STRINGTEMPLATE_NAMESPACE_END

#endif // #ifndef STRINGTEMPLATE_FILE_HPP

