#pragma once

#include <string>

#define STRING(what, def) extern std::string STR_##what;
#include "language.inl"
#undef STRING

bool readLanguage(const char* path);