#include "DialogNode.h"

UDialogNode::UDialogNode() {
    left = nullptr;
    right = nullptr;
    token.tokenType = ETokenType::Node;
}

// UDialogNode::~UDialogNode() {
//     delete left;
//     delete right;
// }

FString UDialogNode::ToString() {
    FString output;
    ToString(output, "", this, false);
    return output;
}

void UDialogNode::ToString(FString& buffer, FString prefix, UDialogNode* n, bool isLeft) {
    if (n) {
        buffer.Append(prefix);
        buffer.Append(isLeft ? "|-- " : "\\-- ");
        buffer.Append(UEnum::GetDisplayValueAsText(token.tokenType).ToString());
        ToString(buffer, prefix + (isLeft ? "|   " : "    "), n->left, true);
        ToString(buffer, prefix + (isLeft ? "|   " : "    "), n->right, false);
    }
}
