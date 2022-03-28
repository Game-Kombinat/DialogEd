#pragma once
#include "Node.h"

namespace DialogEd {
    class FIdentifierNode : public FNode {
    public:
        FString identifierLabel;

        explicit FIdentifierNode(FParsedToken t, FIdentifierNode* identifierType = nullptr);
    };
}

