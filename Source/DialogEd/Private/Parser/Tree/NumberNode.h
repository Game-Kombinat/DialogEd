#pragma once
#include "Node.h"

namespace DialogEd {
    class FNumberNode : public FNode {
    public:
        int value;

        FNumberNode(FParsedToken token);
    };
}

