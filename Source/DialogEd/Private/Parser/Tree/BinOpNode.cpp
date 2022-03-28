#include "BinOpNode.h"

DialogEd::FBinOpNode::FBinOpNode(FParsedToken token, FNode* left, FNode* right) {
    switch (token.tokenType) {
        case ETokenType::Plus:
            opType = EOperatorType::Add;
            break;
        case ETokenType::Minus:
            opType = EOperatorType::Sub;
            break;
        case ETokenType::Slash:
            opType = EOperatorType::Div;
            break;
        case ETokenType::Asterisk:
            opType = EOperatorType::Mult;
            break;
        
        case ETokenType::Less:
            opType = EOperatorType::Less;
            break;
        case ETokenType::Greater:
            opType = EOperatorType::Greater;
            break;
        case ETokenType::LessOrEqual:
            opType = EOperatorType::LessOrEqual;
            break;
        case ETokenType::GreaterOrEqual:
            opType = EOperatorType::GreaterOrEqual;
            break;
        case ETokenType::Equal:
            opType = EOperatorType::Equal;
            break;
        case ETokenType::NotEqual:
            opType = EOperatorType::NotEqual;
            break;
        
        case ETokenType::And:
            opType = EOperatorType::And;
            break;
        case ETokenType::Or:
            opType = EOperatorType::Or;
            break;
        
        default:
            opType = EOperatorType::NoOp;
    }
    this->left = left;
    this->right = right;
}
