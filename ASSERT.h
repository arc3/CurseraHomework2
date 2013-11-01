#ifndef ASSERT_H
#define ASSERT_H

#include <cstdlib>
#include <cstdio>

#define ASSERT(message, condition)  \
    if(!(condition))                \
    {                               \
        printf(message);            \
        exit(1);                    \
    }
#endif // ASSERT_H
