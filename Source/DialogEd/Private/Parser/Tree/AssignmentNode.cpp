#include "AssignmentNode.h"

UAssignmentNode::UAssignmentNode() {
}

UAssignmentNode::UAssignmentNode(FParsedToken t, UDialogNode* lhs, UDialogNode* rhs) {
    Init(t, lhs, rhs);
}

void UAssignmentNode::Init(FParsedToken t, UDialogNode* lhs, UDialogNode* rhs) {
    token = t;
    left = lhs;
    right = rhs;
}
