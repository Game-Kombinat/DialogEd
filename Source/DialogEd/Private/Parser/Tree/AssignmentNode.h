#pragma once
#include "Node.h"

namespace DialogEd {
    class FAssignmentNode : public FNode {
    public:
        FAssignmentNode(FParsedToken t, FNode* lhs, FNode* rhs);
    };
}

