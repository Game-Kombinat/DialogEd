#include "Parser/Tree/TextNode.h"

UTextNode::UTextNode() {
}

UTextNode::UTextNode(FParsedToken t, FString inValue) {
    Init(t, inValue);
}

void UTextNode::Init(FParsedToken t, FString inValue) {
    token = t;
    value = inValue;
}
