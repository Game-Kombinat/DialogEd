#pragma once
#include "IdentifierNode.h"
#include "Node.h"

namespace DialogEd {
    class FCommandNode : public FNode {
        public:
        FString commandName;
        TArray<FIdentifierNode*> argumentList;
    };
}

