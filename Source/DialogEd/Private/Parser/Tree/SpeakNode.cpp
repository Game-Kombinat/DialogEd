#include "Parser/Tree/SpeakNode.h"

#include "Parser/Tree/IdentifierNode.h"
#include "Parser/Tree/TextNode.h"


USpeakNode::USpeakNode() {
    token.tokenType = ETokenType::Speech;
}

USpeakNode::USpeakNode(UIdentifierNode* speaker, UTextNode* text) {
    Init(speaker, text);
}

void USpeakNode::Init(UIdentifierNode* speaker, UTextNode* text) {
    left = speaker;
    right = text;
    token.tokenType = ETokenType::Speech;
}

FString USpeakNode::GetSpeaker() const {
    return static_cast<UIdentifierNode*>(left)->identifierLabel;
}

FString USpeakNode::GetText() const {
    return static_cast<UTextNode*>(right)->value;
}
