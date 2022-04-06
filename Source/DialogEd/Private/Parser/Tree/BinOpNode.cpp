#include "Parser/Tree/BinOpNode.h"

#include "Logging.h"
#include "Parser/Tree/IdentifierNode.h"
#include "Parser/Tree/NumberNode.h"

UBinOpNode::UBinOpNode() {
    opType = EOperatorType::NoOp;
}

UBinOpNode::UBinOpNode(FParsedToken t, UDialogNode* inLeft, UDialogNode* inRight) {
    Init(t, inLeft, inRight);
}

void UBinOpNode::Init(FParsedToken t, UDialogNode* inLeft, UDialogNode* inRight) {
    switch (t.tokenType) {
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
        case ETokenType::SingleEqual:
            opType = EOperatorType::Assignment;
            break;

        default:
            opType = EOperatorType::NoOp;
    }
    this->left = inLeft;
    this->right = inRight;
    token = t;
    token.tokenType = ETokenType::BinOp;
}

int UBinOpNode::Evaluate(UStoryRunner* storyRunner) const {
    // switch on optype
    // compare left with right token values according to op type.
    // use runner to retrieve data context and other maybe required things
    // if optype is assignment return 1 if assignment success, 0 if not successful

    switch (opType) {
        case EOperatorType::NoOp:
            LOG_WARNING("NoOp BinOp node in story found!");
            return 0;
        case EOperatorType::Add:
            return GetLeftValue(storyRunner) + GetRightValue(storyRunner);
        case EOperatorType::Sub:
            return GetLeftValue(storyRunner) - GetRightValue(storyRunner);
        case EOperatorType::Mult:
            return GetLeftValue(storyRunner) * GetRightValue(storyRunner);
        case EOperatorType::Div:
            return GetLeftValue(storyRunner) / FMath::Max(1, GetRightValue(storyRunner));
        case EOperatorType::Less:
            return GetLeftValue(storyRunner) < FMath::Max(1, GetRightValue(storyRunner)) ? 1 : 0;
        case EOperatorType::Greater:
            return GetLeftValue(storyRunner) > FMath::Max(1, GetRightValue(storyRunner)) ? 1 : 0;
        case EOperatorType::LessOrEqual:
            return GetLeftValue(storyRunner) <= FMath::Max(1, GetRightValue(storyRunner)) ? 1 : 0;
        case EOperatorType::GreaterOrEqual:
            return GetLeftValue(storyRunner) >= FMath::Max(1, GetRightValue(storyRunner)) ? 1 : 0;
        case EOperatorType::Equal:
            return GetLeftValue(storyRunner) == FMath::Max(1, GetRightValue(storyRunner)) ? 1 : 0;
        case EOperatorType::NotEqual:
            return GetLeftValue(storyRunner) != FMath::Max(1, GetRightValue(storyRunner)) ? 1 : 0;
        case EOperatorType::And: {
            bool lboolAnd = GetLeftValue(storyRunner) > 0;
            bool rboolAnd = GetRightValue(storyRunner) > 0;
            return lboolAnd && rboolAnd ? 1 : 0;
        }
            
        case EOperatorType::Or: {
            bool lboolOr = GetLeftValue(storyRunner) > 0;
            bool rboolOr = GetRightValue(storyRunner) > 0;
            return lboolOr || rboolOr ? 1 : 0;
        }
            
        case EOperatorType::Assignment:
            return Assign(storyRunner);
        default:
            return 0;
    }
}

int UBinOpNode::GetLeftValue(UStoryRunner* runner) const {
    if (!left) {
        // this is not quite valid. A binop can be a single identifier but
        // it shouldn't be the left node that is missing 
        LOG_WARNING("BinOp: Left Value is null!")
        return 0;
    }

    return GetNodeValue(runner, left);
}

int UBinOpNode::GetRightValue(UStoryRunner* runner) const {
    if (!right) {
        // this is a valid situation, actually. Since a binop node can represent a single identifier.
        return 0;
    }

    return GetNodeValue(runner, right);
}

int UBinOpNode::Assign(UStoryRunner* runner) const {
    runner->GetDataContext()->SetValue(GetLeftIdentifier(), GetRightValue(runner));
    return 1;
}

FString UBinOpNode::GetLeftIdentifier() const {
    if (!left || left->token.tokenType != ETokenType::NumberLiteral) {
        return "";
    }
    auto lval = static_cast<UIdentifierNode*>(left);
    return lval->identifierLabel;
}

int UBinOpNode::GetNodeValue(UStoryRunner* runner, UDialogNode* node) {
    if (node->token.tokenType == ETokenType::Identifier) {
        auto lval = static_cast<UIdentifierNode*>(node);
        return runner->GetDataContext()->GetValue(lval->identifierLabel);
    }
    if (node->token.tokenType == ETokenType::NumberLiteral) {
        auto lval = static_cast<UNumberNode*>(node);
        return lval->value;
    }
    if (node->token.tokenType == ETokenType::BinOp) {
        auto lval = static_cast<UBinOpNode*>(node);
        return lval->Evaluate(runner);
    }
    return 0;
}
