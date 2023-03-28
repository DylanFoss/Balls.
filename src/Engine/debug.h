#pragma once

#define ASSERT(x) if (!(x)) __debugbreak()

#ifdef _DEBUG
#define DEBUGASSERT(x) if (!(x)) __debugbreak()
#else
#define DEBUGASSERT(x) x
#endif

#ifdef _DEBUG
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif