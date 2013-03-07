#ifndef _MOVE_OPERATOR_H
#define _MOVE_OPERATOR_H

#include "Operator.h"
#include <glm/glm.hpp>

class MoveOperator:
    public Operator
{
    public:
        explicit MoveOperator(const std::tuple<
            gtWindowManager*,
            std::string>& args
        ):
            Operator(args)
        {}

        virtual ~MoveOperator() {}

        void move_window(const std::vector<Motion*>& motions);
        void snap_window();

        virtual bool execute() override;
};

#endif

