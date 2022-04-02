#pragma once
#include "Parser/ParsedToken.h"

namespace DialogEd {
    class FNode {
    public:
        FParsedToken token;
        FNode* left;
        FNode* right;
        FNode();
        ~FNode();
        FString ToString();

        void ToString(FString& buffer, FString prefix, FNode* n, bool isLeft);
    };
}
