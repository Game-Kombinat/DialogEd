#include "Node.h"

DialogEd::FNode::FNode() {
    left = nullptr;
    right = nullptr;
}

DialogEd::FNode::~FNode() {
    delete left;
    delete right;
}
