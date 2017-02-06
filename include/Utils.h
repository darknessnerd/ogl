#ifndef UTILS_H
#define UTILS_H

#include <stdarg.h> // used by log functions to have variable number of args
#include <stdio.h>
namespace core

{
namespace utils
{
namespace gl
{
bool log (const char* message, ...)
{

    va_list argptr;
    va_start(argptr, message);
    vfprintf(stdout, message, argptr);
    va_end(argptr);

    return true;
}
}
}
}
#endif // UTILS_H
