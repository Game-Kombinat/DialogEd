#include "DialogModel.h"

#include "Logging.h"
#include "Tree/AssignmentNode.h"
#include "Tree/BinOpNode.h"
#include "Tree/IdentifierNode.h"
#include "Tree/NumberNode.h"
#include "Tree/SpeakNode.h"
#include "Tree/TextNode.h"

void FDialogModel::Make() {
    // manually create starting situation for the token look-around
    currentToken = tokenStack.Pop();
    nextToken = tokenStack.Pop();
    // we start by beginning a thread
    auto root = BeginThread();
    auto node = root;
    while (currentToken.tokenType != ETokenType::Invalid) {
        // each input file can have multiple threads in it, make sure this is accounted for
        // with a special case. It seems we have to do it this way because it's basically ending a parsing step
        // and we have to housekeep some pointers.
        if (currentToken.tokenType == ETokenType::CloseStory) {
            threads.Add(root);
            root = BeginThread();
            node = root;
        }
        node = RunThreadStatements(node);
        if (!node) {
            break;
        }
    }
}

void FDialogModel::Next(ETokenType consume) {
    if (currentToken.tokenType != consume) {
        LOG_ERROR("Unexpected token! Found %s, expected %s", *UEnum::GetDisplayValueAsText(currentToken.tokenType).ToString(), *UEnum::GetDisplayValueAsText(consume).ToString())
        return;
    }
    lastToken = currentToken;
    currentToken = nextToken;
    if (tokenStack.Num() > 0) {
        nextToken = tokenStack.Pop();
    }
    else {
        nextToken = FParsedToken(); // becomes invalid token
    }
    
    // Ignore comments, of course
    if (currentToken.tokenType == ETokenType::Comment) {
        Next(ETokenType::Comment);
    }
    if (currentToken.tokenType == ETokenType::CommentMultiLine) {
        Next(ETokenType::CommentMultiLine);
    }
}

DialogEd::FNode* FDialogModel::BeginThread() {
    Next(ETokenType::OpenStory);
    return new DialogEd::FThreadNode();
}

DialogEd::FNode* FDialogModel::Identifier() {
    
    DialogEd::FIdentifierNode* identifier = new DialogEd::FIdentifierNode(currentToken);
    DialogEd::FNode* node = identifier;
    // if next token it's a colon: this could be a variable type definition (i:inventoryItem).
    // or it could be a speaker name (john: "speak")
    if (nextToken.tokenType == ETokenType::Colon) {
        Next(ETokenType::Identifier); // now current is colon
        
        Next(ETokenType::Colon); // and now current is the token after colon
        if (currentToken.tokenType == ETokenType::Identifier) {
            node = new DialogEd::FIdentifierNode(currentToken, identifier);
            Next(currentToken.tokenType);
        }
    }
    // otherwise this is some kind of binary operator or comparison operator or assignment.
    // either way, this will be evaluated at the callsite.
    Next(currentToken.tokenType);
    return node;
}

DialogEd::FNode* FDialogModel::LiteralOrIdentifier() {
    // valid tokens here are: 
    // * number literal
    // * identifier
    if (currentToken.tokenType == ETokenType::NumberLiteral) {
        const auto t = new DialogEd::FNumberNode(currentToken);
        Next(currentToken.tokenType);
        return t;
    }
    if (currentToken.tokenType == ETokenType::Identifier) {
        return Identifier(); // calls Next() within
    }
    LOG_ERROR("Syntax Error: Expected number literal or identifier, got %s", *UEnum::GetDisplayValueAsText(currentToken.tokenType).ToString());
    Next(currentToken.tokenType);
    return nullptr;
}

DialogEd::FNode* FDialogModel::MathExpression() {
    DialogEd::FNode* lhs = LiteralOrIdentifier();
    if (!lhs) {
        // LiteralOrIdentifier already spat out error msg
        return nullptr;
    }
    while (CurrentTokenIsBinOp()) {
        // check multiplication and division first
        while (currentToken.tokenType == ETokenType::Asterisk || currentToken.tokenType == ETokenType::Slash) {
            const auto opToken = currentToken;
            Next(currentToken.tokenType); // now we're at the RHS of the math op
            // expression should handle identifier and number literals
            const auto rhs = LiteralOrIdentifier();
            if (!rhs) {
                delete lhs;
                return nullptr;
            }
            lhs = new DialogEd::FBinOpNode(opToken, lhs, rhs);
        }

        // and now check additions, because dots before dashes.
        while (currentToken.tokenType == ETokenType::Plus || currentToken.tokenType == ETokenType::Minus) {
            const auto opToken = currentToken;
            Next(currentToken.tokenType); // now we're at the RHS of the math op
            // expression should handle identifier and number literals
            const auto rhs = LiteralOrIdentifier();
            if (!rhs) {
                delete lhs;
                return nullptr;
            }
            lhs = new DialogEd::FBinOpNode(opToken, lhs, rhs);
        }
    }
    return lhs;
}

DialogEd::FNode* FDialogModel::LogicExpression() {
    DialogEd::FNode* lhs = MathExpression();
    if (!lhs) {
        // LiteralOrIdentifier already spat out error msg
        return nullptr;
    }
    while (CurrentTokenIsComparison() || CurrentTokenIsLogicOp()) {
        // first and
        while (currentToken.tokenType == ETokenType::And) {
            const auto opToken = currentToken;
            Next(currentToken.tokenType); // now we're at the RHS of the op
            const auto rhs = MathExpression();
            if (!rhs) {
                delete lhs;
                return nullptr;
            }
            lhs = new DialogEd::FBinOpNode(opToken, lhs, rhs);
        }
        // then or
        while (currentToken.tokenType == ETokenType::Or) {
            const auto opToken = currentToken;
            Next(currentToken.tokenType); // now we're at the RHS of the op
            const auto rhs = MathExpression();
            if (!rhs) {
                delete lhs;
                return nullptr;
            }
            lhs = new DialogEd::FBinOpNode(opToken, lhs, rhs);
        }
        // Then check for comparison operations and put them also in a binop
        // and now check additions, because dots before dashes.
        while (CurrentTokenIsComparison()) {
            const auto opToken = currentToken;
            Next(currentToken.tokenType); // now we're at the RHS of the op
            const auto rhs = MathExpression();
            if (!rhs) {
                delete lhs;
                return nullptr;
            }
            lhs = new DialogEd::FBinOpNode(opToken, lhs, rhs);
        }
    }
    return lhs;
}

DialogEd::FNode* FDialogModel::Text() {
    DialogEd::FNode* lhsIdentifier = Identifier();
    const auto idNode = static_cast<DialogEd::FIdentifierNode*>(lhsIdentifier);
    if (!idNode) {
        LOG_ERROR("Syntax error: Expected Identifier before Text, got %s", *UEnum::GetDisplayValueAsText(lhsIdentifier->token.tokenType).ToString());
        delete lhsIdentifier;
        Next(currentToken.tokenType);
        return nullptr;
    }
    const auto dt = new DialogEd::FTextNode(currentToken);
    Next(currentToken.tokenType);
    return new DialogEd::FSpeakNode(idNode, dt);
}

DialogEd::FNode* FDialogModel::Assignment() {
    DialogEd::FNode* lhsIdentifier = Identifier();
    bool hadRun = false;
    while (currentToken.tokenType == ETokenType::SingleEqual && lhsIdentifier->token.tokenType == ETokenType::Identifier) {
        // assignment
        // now, we know the left hand side, it's an identifier, has to be for an assignment.
        // but the RHS could be a number literal, or another identifier or a whole expression.
        Next(currentToken.tokenType); // current is now at token after =
        hadRun = true;
        
        if (NextTokenIsBinOp()) {
            // this will be simply a = 5 + 5 or a = b + c - 3 etc. also a = b = 5+3
            lhsIdentifier = new DialogEd::FAssignmentNode(lastToken, lhsIdentifier, MathExpression());
        }
        else if (NextTokenIsComparison() || NextTokenIsLogicOp()) {
            // this ends up being like a = b < 5 etc which is coerced into integers. truthyness interpretation not up to AST
            // also possible would be a = b < 5 && d > 3 || c
            lhsIdentifier = new DialogEd::FAssignmentNode(lastToken, lhsIdentifier, LogicExpression());
        }
        else if (nextToken.tokenType == ETokenType::Identifier) {
            // and this is just a normal a = b kinda situation
            lhsIdentifier = new DialogEd::FAssignmentNode(lastToken, lhsIdentifier, Identifier());
        }
        
    }
    if (!hadRun) {
        LOG_ERROR("Syntax Error. Expected Assignment but got %s", *UEnum::GetDisplayValueAsText(currentToken.tokenType).ToString())
        Next(currentToken.tokenType);
        delete lhsIdentifier;
        return nullptr;
    }
    // last MathExpression already called Next()
    return lhsIdentifier;
}

DialogEd::FNode* FDialogModel::AssignmentOrText() {
    if (nextToken.tokenType == ETokenType::Text) {
        // speech
        return Text();
    }
    if (nextToken.tokenType == ETokenType::SingleEqual) {
        // account for successive assignments
        return Assignment();
    }

    LOG_ERROR("Syntax Error: Unexpected symbols. Got %s %s", *UEnum::GetDisplayValueAsText(currentToken.tokenType).ToString(), *UEnum::GetDisplayValueAsText(nextToken.tokenType).ToString());
    Next(currentToken.tokenType);
    return nullptr;
}

DialogEd::FNode* FDialogModel::RunThreadStatements(DialogEd::FNode* node) {
    node->left = Statement();
    if (!node->left || node->left->token.tokenType == ETokenType::Invalid) {
        // todo: should analyse the tokens here and see with which one it went derp.
        LOG_ERROR("Unknown / invalid token discovered: %s", *UEnum::GetDisplayValueAsText(lastToken.tokenType).ToString());
        delete node;
        return nullptr;
    }
    node->right = new DialogEd::FThreadNode();
    node = node->right;
    return node;
}

DialogEd::FNode* FDialogModel::If() {
    /*
     * If node:
     * left: Logic Expression
     * right: FNode: left: if true (ThreadNode), right: else (ThreadNode)
     */
    const auto ifNode = new DialogEd::FNode();
    ifNode->token = currentToken;
    Next(currentToken.tokenType);
    // we're now at the spot after the if, which can only be a logic expression
    const auto expression = LogicExpression();
    if (!expression) {
        // ... and if it ain't, this is an epic fail
        delete ifNode;
        return nullptr;
    }
    ifNode->left = expression;
    DialogEd::FNode* node = new DialogEd::FThreadNode();
    DialogEd::FNode* ifBranches = new DialogEd::FNode();
    ifBranches->left = node; // this is going to be the true-branch and right is the false branch
    ifNode->right = ifBranches;

     bool failed = false;
    
    // now we have to assemble the if branch until we hit either else or endif
    // run the true-branchNext(currentToken.tokenType);
    while (currentToken.tokenType != ETokenType::Else || currentToken.tokenType != ETokenType::Endif) {
        node = RunThreadStatements(node);
        if (!node) {
            failed = true;
            break;
        }
    }
    if (failed) {
        delete ifNode;
        return nullptr;
    }
    // run the else branch if it exists
    if (currentToken.tokenType == ETokenType::Else) {
        Next(ETokenType::Else);
        node = new DialogEd::FThreadNode();
        ifBranches->right = node;
        while (currentToken.tokenType != ETokenType::Endif) {
            node = RunThreadStatements(node);
            if (!node) {
                failed = true;
                break;
            }
        }
    }
    Next(ETokenType::Endif);
    if (failed) {
        delete ifNode;
        return nullptr;
    }
    return ifNode;
}

DialogEd::FNode* FDialogModel::Statement() {
    // at this point, the only valid tokens are:
    // * Identifier
    // * if
    // * command
    // * choice
    // * pre-inc
    // * pre-dec
    switch (currentToken.tokenType) {
        case ETokenType::Identifier:
            // as root element, an identifier can only lead into an assignment or into a text.
            return AssignmentOrText();
        case ETokenType::If:
            return If();
        case ETokenType::Command:
            break;
        case ETokenType::BeginBranching:
            break;
        case ETokenType::PlusPlus:
            break;
        case ETokenType::MinusMinus:
            break;
        default:
            const FString enumType = UEnum::GetDisplayValueAsText(currentToken.tokenType).ToString();
            LOG_ERROR("Invalid %s found. Check Syntax near '%s'", *enumType, *currentToken.value);
            return nullptr;
    }

    // to make rider shut up while the switch case is incomplete
    LOG_ERROR("Invalid %s found. Check Syntax near '%s'", *UEnum::GetDisplayValueAsText(currentToken.tokenType).ToString(), *currentToken.value);
    return nullptr;
}

bool FDialogModel::CurrentTokenIsBinOp() const {
    return currentToken.tokenType == ETokenType::Plus ||
        currentToken.tokenType == ETokenType::Minus ||
        currentToken.tokenType == ETokenType::Slash ||
        currentToken.tokenType == ETokenType::Asterisk;
}

bool FDialogModel::CurrentTokenIsComparison() const {
    return currentToken.tokenType == ETokenType::Greater ||
        currentToken.tokenType == ETokenType::GreaterOrEqual ||
        currentToken.tokenType == ETokenType::Less ||
        currentToken.tokenType == ETokenType::LessOrEqual ||
        currentToken.tokenType == ETokenType::Equal ||
        currentToken.tokenType == ETokenType::NotEqual;
}

bool FDialogModel::CurrentTokenIsLogicOp() const {
    return currentToken.tokenType == ETokenType::And ||
        currentToken.tokenType == ETokenType::Or;
}

bool FDialogModel::NextTokenIsBinOp() const {
    return nextToken.tokenType == ETokenType::Plus ||
        nextToken.tokenType == ETokenType::Minus ||
        nextToken.tokenType == ETokenType::Slash ||
        nextToken.tokenType == ETokenType::Asterisk;
}

bool FDialogModel::NextTokenIsComparison() const {
    return nextToken.tokenType == ETokenType::Greater ||
        nextToken.tokenType == ETokenType::GreaterOrEqual ||
        nextToken.tokenType == ETokenType::Less ||
        nextToken.tokenType == ETokenType::LessOrEqual ||
        nextToken.tokenType == ETokenType::Equal ||
        nextToken.tokenType == ETokenType::NotEqual;
}

bool FDialogModel::NextTokenIsLogicOp() const {
    return nextToken.tokenType == ETokenType::And ||
        nextToken.tokenType == ETokenType::Or;
}
