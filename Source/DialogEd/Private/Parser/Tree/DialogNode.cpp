#include "Parser/Tree/DialogNode.h"

UDialogNode::UDialogNode() {
    left = nullptr;
    right = nullptr;
    token.tokenType = ETokenType::Node;
}

FString UDialogNode::ToString() {
    return FString::Format(TEXT("{\n{0}\n}"), {ToString(1)});
}

FString UDialogNode::ToString(int indent) {
    FString output;
    FString indentation;
    for (int i = 0; i < indent; ++i) {
        indentation.Append("\t");
    }
    output.Append(FString::Format(TEXT("{\n{3}\"type\": \"{0}\",\n{3}\"left\": {1},\n{3}\"right\": {2},\n{3}}"), {
        UEnum::GetDisplayValueAsText(token.tokenType).ToString(),
        left ? left->ToString(indent + 1) : "{}",
        right ? right->ToString(indent + 1) : "{}",
        indentation
        
    }));
    return output;
}
