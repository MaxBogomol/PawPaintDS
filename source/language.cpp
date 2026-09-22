#include "language.h"

#include <nds.h>
#include <map>

using namespace std;

#define STRING(what, def) string STR_##what;
#include "language.inl"
#undef STRING

bool readLanguage(const char* path) {
    FILE* fp = fopen(path, "rb");
    if (!fp) {
        #define STRING(what, def) STR_##what = def;
        #include "language.inl"
        #undef STRING
        return false;
    }

    map<string, string> keys;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char key[100], value[100];
        if (sscanf(line, "%99[^=]=%99[^\n]", key, value) == 2) {
            string k = key;
            string v = value;
            if (v.back() == '\r') v.pop_back();
            if (!k.empty()) keys[k] = v;
        }
    }

    #define STRING(what, def) STR_##what = (keys.find(#what) != keys.end()) ? keys[#what] : def;
    #include "language.inl"
    #undef STRING

    fclose(fp);

    return true;
}