#ifndef MACROS_H_
#define MACROS_H_

#include <stdlib.h>

#define FREE(ptr) do {                \
                      free(ptr);      \
                      ptr = nullptr;  \
                  } while (0)

#endif // #ifndef MACROS_H_
