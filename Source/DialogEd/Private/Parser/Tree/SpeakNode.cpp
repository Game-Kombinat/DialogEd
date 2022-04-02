﻿#include "SpeakNode.h"

#include "IdentifierNode.h"
#include "TextNode.h"

DialogEd::FSpeakNode::FSpeakNode(FIdentifierNode* speaker, FTextNode* text) {
    right = speaker;
    left = text;
    token = FParsedToken(ETokenType::Speech, "", 0, 0, 0);
}
