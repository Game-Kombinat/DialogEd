#include "ChoiceNode.h"

UChoiceNode::UChoiceNode(const FParsedToken t) {
    Init(t);
}

UChoiceNode::UChoiceNode() {
}

void UChoiceNode::Init(const FParsedToken t) {
    choiceLabel = t.value.TrimChar('"');
    token = t;
}
