#include "Node.h"

DialogEd::FNode::FNode() {
    left = nullptr;
    right = nullptr;
    token.tokenType = ETokenType::Node;
}

DialogEd::FNode::~FNode() {
    delete left;
    delete right;
}

FString DialogEd::FNode::ToString() {
    FString output;
    ToString(output, "", this, false);
    return output;
}

void DialogEd::FNode::ToString(FString& buffer, FString prefix, FNode* n, bool isLeft) {
    if (n) {
        buffer.Append(prefix);
        buffer.Append(isLeft ? "|-- " : "\\-- ");
        buffer.Append(UEnum::GetDisplayValueAsText(token.tokenType).ToString());
        ToString(buffer, prefix + (isLeft ? "|   " : "    "), n->left, true);
        ToString(buffer, prefix + (isLeft ? "|   " : "    "), n->right, false);
    }
}
