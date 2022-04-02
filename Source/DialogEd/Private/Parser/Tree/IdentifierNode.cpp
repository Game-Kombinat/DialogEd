#include "IdentifierNode.h"

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
