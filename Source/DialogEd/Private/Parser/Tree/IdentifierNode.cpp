#include "Parser/Tree/IdentifierNode.h"

UIdentifierNode::UIdentifierNode() {
    guid = FGuid::NewGuid();
}

UIdentifierNode::UIdentifierNode(FParsedToken t, UIdentifierNode* identifierType) {
    Init(t, identifierType);
    guid = FGuid::NewGuid();
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

FGuid& UIdentifierNode::GetGuid() {
    return guid;
}
