#include "NumberNode.h"

#include "Kismet/KismetStringLibrary.h"

DialogEd::FNumberNode::FNumberNode(FParsedToken token) {
    this->token = token;
    value = UKismetStringLibrary::Conv_StringToInt(token.value);
}
