#include "Parser/Tree/IdentifierNode.h"

UIdentifierNode::UIdentifierNode() {
}

UIdentifierNode::UIdentifierNode(FParsedToken t, UIdentifierNode* identifierType) {
    Init(t, identifierType);
}

void UIdentifierNode::Init(FParsedToken t, UIdentifierNode* identifierType) {
    token = t;
    identifierLabel = t.value;
    right = identifierType;
}

FString UIdentifierNode::GetIdentifierType() const {
    if (right) {
        if (const auto id = static_cast<UIdentifierNode*>(right)) {
            return id->identifierLabel;
        }
    }
    return "";
}
