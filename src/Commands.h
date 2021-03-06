#ifndef _COMMANDS_DEF_H_G6UPB6OZ
#define _COMMANDS_DEF_H_G6UPB6OZ

#include <string>
#include <vector>
#include "Motions.h"
#include "Operators.h"
#include "Hints.h"
#include "Actions.h"

/*
 * Interleaved enum containing all command IDs
 */
enum class eCommand : unsigned
{
    MOTION, // -> motion IDs start here! (see Motions.h)
    MOTIONS_END = (MOTION + (unsigned)eMotion::MAX - 1),

    OPERATOR, // -> operator IDs start here! (see Operators.h)
    OPERATORS_END = (OPERATOR + (unsigned)eOperator::MAX - 1),

    HINT, // -> hint IDs start here! (see Hints.h)
    HINTS_END = (HINT + (unsigned)eHint::MAX - 1),

    ACTION,
    ACTIONS_END = (ACTION + (unsigned)eAction::MAX - 1),

    MAX
};

#endif

