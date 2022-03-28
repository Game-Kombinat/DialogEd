#pragma once
#include "Node.h"

namespace DialogEd {
    class FTextNode : public FNode {
    public:
        FString value;
        explicit FTextNode(FParsedToken t);
    };
}

