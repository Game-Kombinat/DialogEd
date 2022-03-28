#include "AssignmentNode.h"

DialogEd::FAssignmentNode::FAssignmentNode(FParsedToken t, FNode* lhs, FNode* rhs) {
    token = t;
    left = lhs;
    right = rhs;
}
