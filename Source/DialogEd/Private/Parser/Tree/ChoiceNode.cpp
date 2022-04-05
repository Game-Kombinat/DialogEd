#include "Parser/Tree/ChoiceNode.h"

UChoiceNode::UChoiceNode(const FParsedToken t) {
    Init(t);
}

UChoiceNode::UChoiceNode() {
}

void UChoiceNode::Init(const FParsedToken t) {
    choiceLabel = t.value.TrimChar('"');
    token = t;
}

TMap<int, FString> UChoiceNode::GetChoices() const {
    TMap<int, FString> choices;
    choices.Add(0, choiceLabel);
    auto node = this;
    int nesting = 0;
    while (node) {
        choices.Add(nesting++, node->choiceLabel);
        node = static_cast<UChoiceNode*>(node->right);
    }
    return choices;
}

UChoiceNode* UChoiceNode::GetChoice(const int choice) {
    int iterations = 0;
    auto node = this;
    while (iterations < choice) {
        node = static_cast<UChoiceNode*>(node->right);
        ++iterations;
    }

    return node;
}

UDialogNode* UChoiceNode::GetBranch() const {
    return left;
}
