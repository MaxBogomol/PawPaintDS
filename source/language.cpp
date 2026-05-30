#include "language.h"

#include <nds.h>
#include <filesystem.h>
#include <fat.h>
#include <string>

#define STRING(what, def) std::string STR_##what;
#include "language.inl"
#undef STRING

std::string getString(FILE* &fp, const std::string &item, const std::string &defaultValue) {
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char key[100], value[100];
        if (sscanf(line, "%99[^=]=%99[^\n]", key, value) == 2) {
            if (strcmp(key, item.c_str()) == 0) {
                std::string str = strdup(value);
                if (str.back() == '\r') str.pop_back();
                return str;
            }
        }
    }
    return "";
}

bool readLanguage(const char* path) {
    FILE* fp = fopen(path, "rb");
    if (!fp) return false;

    #define STRING(what, def) STR_##what = getString(fp, ""#what, def);
    #include "language.inl"
    #undef STRING

    return true;
}