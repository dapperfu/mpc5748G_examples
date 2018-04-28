#define START _startcore2
#define __SP_INIT __SP_INIT2
#define MAIN main2
#undef START_FROM_FLASH
#undef I_CACHE
#undef D_CACHE

#include "startup.S"
