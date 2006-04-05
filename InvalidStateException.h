#ifndef SPUG_INVALIDSTATEEXCEPTION_H
#define SPUG_INVALIDSTATEEXCEPTION_H

#include "Exception.h"

namespace spug {

/**
 * This can be thrown any time an operation is performed on an object that is
 * in an inappropriate state.
 */
SPUG_EXCEPTION(InvalidStateException)

} // namespace spug

#endif
