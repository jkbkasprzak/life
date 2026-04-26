#ifndef _GOL_DEBUG_H
#define _GOL_DEBUG_H

#include <assert.h>

/// @brief Check condition in debug configuration
#define GOL_ASSERT(condition, msg) assert((msg, (condition)))

#endif
