#include "IdentifierNode.h"

DialogEd::FIdentifierNode::FIdentifierNode(FParsedToken t, FIdentifierNode* identifierType) {
    token = t;
    identifierLabel = t.value;
    right = identifierType;
}
