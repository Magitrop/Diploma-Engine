#pragma once

#include <cassert>

#if IS_DEBUG
#define DEBUG_ASSERT(expr) if (!(expr)) do { FATAL_LOG("Assertion failed: " #expr); assert(expr); } while(false);
#else
#define DEBUG_ASSERT(expr)
#endif // #if !IS_DEBUG