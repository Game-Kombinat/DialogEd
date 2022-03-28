#pragma once
#include "Node.h"

namespace DialogEd {
    class FSpeakNode : public FNode {
    public:
        FSpeakNode(class FIdentifierNode* speaker, class FTextNode* text);
    };
}

