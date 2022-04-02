#include "NumberNode.h"

#include "Kismet/KismetStringLibrary.h"

UNumberNode::UNumberNode() {
    value = 0;
}

UNumberNode::UNumberNode(FParsedToken token) {
    this->token = token;
    value = UKismetStringLibrary::Conv_StringToInt(token.value);
}

void UNumberNode::Init(FParsedToken t) {
    token = t;
    value = UKismetStringLibrary::Conv_StringToInt(t.value);
}
