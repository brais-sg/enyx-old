#ifndef _DEBUG_INCLUDED
#define _DEBUG_INCLUDED
#include <stdarg.h>

namespace Debug {
    void info   (const char* format, ...);
    void warning(const char* format, ...);
    void error  (const char* format, ...);

    void critical(const char* format, ...);
};

#endif