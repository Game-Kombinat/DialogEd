#pragma once
#include "Node.h"

namespace DialogEd {
    enum class EOperatorType : uint8 {
        NoOp,
        Add,
        Sub,
        Mult,
        Div,
        Less,
        Greater,
        LessOrEqual,
        GreaterOrEqual,
        Equal,
        NotEqual,
        And,
        Or
    };
    class FBinOpNode : public FNode {
    public:
        EOperatorType opType;
        FBinOpNode(FParsedToken t, FNode* left, FNode* right);
    };
}

