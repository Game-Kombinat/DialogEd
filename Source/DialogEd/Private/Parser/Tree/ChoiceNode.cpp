#include "ChoiceNode.h"

DialogEd::FChoiceNode::FChoiceNode(const FParsedToken t) {
    SetTokenAndLabel(t);
}

DialogEd::FChoiceNode::FChoiceNode() {
}

void DialogEd::FChoiceNode::SetTokenAndLabel(const FParsedToken t) {
    choiceLabel = t.value.TrimChar('"');
    token = t;
}
