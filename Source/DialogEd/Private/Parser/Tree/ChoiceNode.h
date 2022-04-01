#pragma once
#include "Node.h"

namespace DialogEd {
    class FChoiceNode : public FNode {
    public:
        FString choiceLabel;
        explicit FChoiceNode(FParsedToken t);
        FChoiceNode();
        void SetTokenAndLabel(FParsedToken t);
    };
}

