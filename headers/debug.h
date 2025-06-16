#pragma once

#include <iostream>

#ifdef NDEBUG
#define DEBUG_PRINT(x)
#else
#define DEBUG_PRINT(x)                                                         \
  do {                                                                         \
    std::cerr << "[DEBUG] " << __FILE__ << ":" << __LINE__ << " (" << __func__ \
              << "): " << x << std::endl;                                      \
  } while (0)
#endif
