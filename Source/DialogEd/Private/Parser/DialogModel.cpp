#include "DialogModel.h"

#include "Logging.h"
#include "Tree/AssignmentNode.h"
#include "Tree/BinOpNode.h"
#include "Tree/ChoiceNode.h"
#include "Tree/CommandNode.h"
#include "Tree/IdentifierNode.h"
#include "Tree/NumberNode.h"
#include "Tree/SpeakNode.h"
#include "Tree/TextNode.h"

FDialogModel::FDialogModel(const TArray<FParsedToken> tokens) {
    tokenStack = tokens;
    Algo::Reverse(tokenStack);
}


// FDialogModel::~FDialogModel() {
//     for (auto i : threads) {
//         delete i;
//     }
// }

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
            Next(ETokenType::CloseStory);
            if (currentToken.tokenType == ETokenType::Invalid) {
                // we have reached the end of this input file
                break;
            }
            root = BeginThread();
            node = root;
        }
        
        node = RunThreadStatement(node);
        if (!node) {
            break;
        }
    }
}

FString FDialogModel::ToString() {
    FString buffer;
    for (int i = 0; i < threads.Num(); ++i) {
        buffer.Append(threads[i]->ToString());
        buffer.Append("\n");
    }

    return buffer;
}

void FDialogModel::TreeToString(UDialogNode* root, FString& buffer) {
    struct ListOfLines {
        TArray<FString> list;
    };
    TArray<ListOfLines> lines;
    TArray<UDialogNode*> level;
    TArray<UDialogNode*> next;
    level.Add(root);
    int nn = 1;
    int widest = 0;
    // collect the display data from the tree.
    while (nn != 0) {
        ListOfLines line;
        nn = 0;
        // auto node : level
        for (int i = 0; i < level.Num(); ++i) {
            auto node = level[i];
            if (!node) {
                line.list.Add(FString());
                next.Add(nullptr);
                next.Add(nullptr);
            }
            else {
                FString display = UEnum::GetDisplayValueAsText(node->token.tokenType).ToString();
                line.list.Add(display);
                if (display.Len() > widest) {
                    widest = display.Len();
                }
                next.Add(node->left);
                next.Add(node->right);
                if (node->left) {
                    nn++;
                }
                if (node->right) {
                    nn++;
                }
            }
        }

        if (widest % 2 == 1) {
            widest++;
        }
        lines.Add(line);
        auto tmp = level;
        level = next;
        next = tmp;
        next.Reset();
    }

    // print it.
    int perPiece = lines.Last().list.Num() * (widest + 4);

    for (int i = 0; i < lines.Num(); ++i) {
        TArray<FString> line = lines[i].list;
        int hpw = FMath::FloorToInt(perPiece / 2.0f) - 1;

        if (i > 0) {
            for (int j = 0; j < line.Num(); ++j) {
                // split node
                FString c = " ";
                
                if (j % 2 == 1) {
                    if (!line[j - 1].IsEmpty()) {
                        c = (!line[j].IsEmpty() ? "|" : "-|");
                    }
                    else {
                        if (j < line.Num() && !line[j].IsEmpty()) {
                            c = "|-";
                        }
                    }
                }
                buffer.Append(c);

                // lines and spaces
                if (line[j].IsEmpty()) {
                    for (int k = 0; k < perPiece - 1; ++k) {
                        buffer.Append(" ");
                    }
                }
                else {
                    //buffer.AppendChars()
                    for (int k = 0; k < hpw; k++) {
                        buffer.Append(j % 2 == 0 ? " " : "-");
                    }
                    buffer.Append(j % 2 == 0 ? "|-" : "-|");
                    for (int k = 0; k < hpw; k++) {
                        buffer.Append(j % 2 == 0 ? "-" : " ");
                    }
                }
            }
            buffer.Append("\n");
        }
        // print line of numbers
        for (int j = 0; j < line.Num(); ++j) {

            FString f = line[j];
            
            int gap1 = FMath::CeilToInt(perPiece / 2.0f - f.Len() / 2.0f);
            int gap2 = FMath::FloorToInt(perPiece / 2.0f - f.Len() / 2.0f);

            // a number
            for (int k = 0; k < gap1; k++) {
                buffer.Append(" ");
            }
            buffer.Append(f);
            for (int k = 0; k < gap2; k++) {
                buffer.Append(" ");
            }
        }
        buffer.Append("\n");

        perPiece /= 2;
    }
}

void FDialogModel::Next(ETokenType consume) {
    if (currentToken.tokenType != consume) {
        LOG_ERROR("Next: Unexpected token! Found %s, expected %s", *UEnum::GetDisplayValueAsText(currentToken.tokenType).ToString(), *UEnum::GetDisplayValueAsText(consume).ToString())
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

UDialogNode* FDialogModel::BeginThread() {
    LOG_INFO("Opening new MainThread");
    Next(ETokenType::OpenStory);
    return NewObject<UDialogNode>();//  new UNode();
}

UDialogNode* FDialogModel::Identifier() {

    UIdentifierNode* identifier = NewObject<UIdentifierNode>();// new UIdentifierNode(currentToken);
    identifier->Init(currentToken);
    
    UDialogNode* node = identifier;
    // if next token it's a colon: this could be a variable type definition (i:inventoryItem).
    // or it could be a speaker name (john: "speak")
    if (nextToken.tokenType == ETokenType::Colon) {
        Next(ETokenType::Identifier); // now current is colon

        Next(ETokenType::Colon); // and now current is the token after colon
        if (currentToken.tokenType == ETokenType::Identifier) {
            node = NewObject<UIdentifierNode>(); //new UIdentifierNode(currentToken, identifier);
            static_cast<UIdentifierNode*>(node)->Init(currentToken, identifier);
            Next(currentToken.tokenType);
        }
    }
    else {
        Next(ETokenType::Identifier);
    }
    // otherwise this is some kind of binary operator or comparison operator or assignment.
    // either way, this will be evaluated at the callsite.
    return node;
}

UDialogNode* FDialogModel::LiteralOrIdentifier() {
    // valid tokens here are: 
    // * number literal
    // * identifier
    if (currentToken.tokenType == ETokenType::NumberLiteral) {
        const auto t = NewObject<UNumberNode>();// new UNumberNode(currentToken);
        t->Init(currentToken);
        Next(ETokenType::NumberLiteral);
        return t;
    }
    if (currentToken.tokenType == ETokenType::Identifier) {
        return Identifier(); // calls Next() within
    }
    LOG_ERROR("Syntax Error: Expected number literal or identifier, got %s", *UEnum::GetDisplayValueAsText(currentToken.tokenType).ToString());
    Next(currentToken.tokenType);
    return nullptr;
}

UDialogNode* FDialogModel::MathExpression() {
    // todo: we could use parenthesis to force operator precedence some day
    UDialogNode* lhs = LiteralOrIdentifier();
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
                return nullptr;
            }
            auto tmp = NewObject<UBinOpNode>();
            tmp->Init(opToken, lhs, rhs);
            lhs = tmp; // new UBinOpNode(opToken, lhs, rhs);
            
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
            auto tmp = NewObject<UBinOpNode>();
            tmp->Init(opToken, lhs, rhs);
            lhs = tmp; // new UBinOpNode(opToken, lhs, rhs);
        }
    }
    return lhs;
}

UDialogNode* FDialogModel::LogicExpression() {
    // todo: we could use parenthesis to force operator precedence some day
    UDialogNode* lhs = MathExpression();
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
            auto tmp = NewObject<UBinOpNode>();
            tmp->Init(opToken, lhs, rhs);
            lhs = tmp; // new UBinOpNode(opToken, lhs, rhs);
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
            auto tmp = NewObject<UBinOpNode>();
            tmp->Init(opToken, lhs, rhs);
            lhs = tmp; // new UBinOpNode(opToken, lhs, rhs);
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
            auto tmp = NewObject<UBinOpNode>();
            tmp->Init(opToken, lhs, rhs);
            lhs = tmp; // new UBinOpNode(opToken, lhs, rhs);
        }
    }
    return lhs;
}

UDialogNode* FDialogModel::Text(UDialogNode* lhsIdentifier) {
    const auto idNode = static_cast<UIdentifierNode*>(lhsIdentifier);
    if (!idNode) {
        LOG_ERROR("Syntax error: Expected Identifier before Text, got %s", *UEnum::GetDisplayValueAsText(lhsIdentifier->token.tokenType).ToString());
        delete lhsIdentifier;
        Next(currentToken.tokenType);
        return nullptr;
    }
    FString inValue = currentToken.value.TrimChar('"');
    const FParsedToken textBaseToken = currentToken;
    Next(ETokenType::Text);
    // thing is, it is syntactically allowed to have another text token here that is supposed to be merged with the previous one.
    while (currentToken.tokenType == ETokenType::Text) {
        FString newLine = FString("\n").Append(currentToken.value.TrimChar('"'));
        inValue.Append(newLine);
        Next(ETokenType::Text);
    }

    auto speakNode = NewObject<USpeakNode>();
    auto textNode = NewObject<UTextNode>();
    textNode->Init(textBaseToken, inValue);
    
    speakNode->Init(idNode, textNode);
    
    return speakNode;
}

UDialogNode* FDialogModel::Assignment(UDialogNode* lhsIdentifier) {
    bool hadRun = false;
    while (currentToken.tokenType == ETokenType::SingleEqual && lhsIdentifier->token.tokenType == ETokenType::Identifier) {
        // assignment
        // now, we know the left hand side, it's an identifier, has to be for an assignment.
        // but the RHS could be a number literal, or another identifier or a whole expression.
        Next(currentToken.tokenType); // current is now at token after =
        hadRun = true;

        // todo: evaluate if we can't do this with just the LogicExpression because it would return a single math expression or number literal
        // if there is no logic expression nearby.
        if (NextTokenIsBinOp()) {
            // this will be simply a = 5 + 5 or a = b + c - 3 etc. also a = b = 5+3
            auto tmp = NewObject<UAssignmentNode>(); // new UAssignmentNode(lastToken, lhsIdentifier, MathExpression());
            tmp->Init(lastToken, lhsIdentifier, MathExpression());
            lhsIdentifier = tmp;
        }
        else if (NextTokenIsComparison() || NextTokenIsLogicOp()) {
            auto tmp = NewObject<UAssignmentNode>(); // new UAssignmentNode(lastToken, lhsIdentifier, MathExpression());
            tmp->Init(lastToken, lhsIdentifier, LogicExpression());
            lhsIdentifier = tmp;
            // this ends up being like a = b < 5 etc which is coerced into integers. truthyness interpretation not up to AST
            // also possible would be a = b < 5 && d > 3 || c
            // lhsIdentifier = new UAssignmentNode(lastToken, lhsIdentifier, LogicExpression());
        }
        else if (nextToken.tokenType == ETokenType::Identifier) {
            // and this is just a normal a = b kinda situation
            auto tmp = NewObject<UAssignmentNode>(); // new UAssignmentNode(lastToken, lhsIdentifier, MathExpression());
            tmp->Init(lastToken, lhsIdentifier, Identifier());
            lhsIdentifier = tmp;
        }

    }
    if (!hadRun) {
        LOG_ERROR("Syntax Error. Expected Assignment but got %s", *UEnum::GetDisplayValueAsText(currentToken.tokenType).ToString())
        Next(currentToken.tokenType);
        return nullptr;
    }
    // last MathExpression already called Next()
    return lhsIdentifier;
}

UDialogNode* FDialogModel::AssignmentOrText() {
    LOG_ERROR("AssignmentOrText: Beginning with %s and %s: \"%s%s\"",
        *UEnum::GetDisplayValueAsText(currentToken.tokenType).ToString(),
        *UEnum::GetDisplayValueAsText(nextToken.tokenType).ToString(),
        *currentToken.value,
        *nextToken.value
        );
    // this will get the identifier however shaped (normal or with type info)
    // it will, in any case, strip the colon if there is one.
    auto identifier = Identifier();
   
    if (currentToken.tokenType == ETokenType::Text) {
        return Text(identifier);
    }
    if (currentToken.tokenType == ETokenType::SingleEqual) {
        // account for successive assignments
        return Assignment(identifier);
    }
    // todo: this can also be a post increment or post decrement, might implement later

    LOG_ERROR("AssignmentOrText: Unexpected symbols. Got %s and %s: \"%s%s\"",
        *UEnum::GetDisplayValueAsText(currentToken.tokenType).ToString(),
        *UEnum::GetDisplayValueAsText(nextToken.tokenType).ToString(),
        *currentToken.value,
        *nextToken.value
        );
    Next(currentToken.tokenType);
    return nullptr;
}

UDialogNode* FDialogModel::RunThreadStatement(UDialogNode* node) {
    LOG_INFO("Handling token %s", *UEnum::GetDisplayValueAsText(currentToken.tokenType).ToString());
    node->left = Statement();
    if (!node->left || node->left->token.tokenType == ETokenType::Invalid) {
        // todo: should analyse the tokens here and see with which one it went derp.
        LOG_ERROR("RunThreadStatement: Unknown / invalid token discovered: %s - check syntax around %s %s %s",
            *UEnum::GetDisplayValueAsText(lastToken.tokenType).ToString(),
            *lastToken.value,
            *currentToken.value,
            *nextToken.value
        );
        return nullptr;
    }
    node->right = NewObject<UDialogNode>(); //  new UNode();
    node = node->right;
    return node;
}

UDialogNode* FDialogModel::If() {
    /*
     * If node:
     * left: Logic Expression
     * right: FNode: left: if true (ThreadNode), right: else (ThreadNode)
     */
    const auto ifNode = NewObject<UDialogNode>(); // new UNode();
    ifNode->token = currentToken;
    Next(ETokenType::If);
    // we're now at the spot after the if, which can only be a logic expression
    const auto expression = LogicExpression();
    if (!expression) {
        // ... and if it ain't, this is an epic fail
        delete ifNode;
        return nullptr;
    }
    ifNode->left = expression;
    UDialogNode* node = NewObject<UDialogNode>(); //new UNode();
    UDialogNode* ifBranches = NewObject<UDialogNode>();
    ifBranches->left = node; // this is going to be the true-branch and right is the false branch
    ifNode->right = ifBranches;

    bool failed = false;

    // now we have to assemble the if branch until we hit either else or endif
    // run the true-branch
    while (currentToken.tokenType != ETokenType::Else && currentToken.tokenType != ETokenType::Endif) {
        if (currentToken.tokenType == ETokenType::Else) {
            break;
        }
        node = RunThreadStatement(node);
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
        node = NewObject<UDialogNode>();
        ifBranches->right = node;
        while (currentToken.tokenType != ETokenType::Endif) {
            node = RunThreadStatement(node);
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

UDialogNode* FDialogModel::Command() {
    Next(ETokenType::Command);
    // now we get identifier, lparen [identifier identifier ...], rparen
    const auto cmdName = static_cast<UIdentifierNode*>(Identifier());
    const auto cmd = NewObject<UCommandNode>(); // new UCommandNode();
    cmd->left = cmdName;
    
    Next(ETokenType::LParen);
    bool failed = false;
    UDialogNode* argNode = nullptr;
    while (currentToken.tokenType != ETokenType::RParen) {
        // This can be an identifier or a math expression or a logic expression
        auto newArg = LogicExpression();
        if (!newArg) {
            failed = true;
            break;
        }
        if (!cmd->right) {
            argNode = newArg;
            cmd->right = argNode;
        }
        else {
            argNode->left = newArg;
            argNode = newArg;
        }
    }
    if (failed) {
        delete cmd;
        return nullptr;
    }

    Next(ETokenType::RParen);

    return cmd;
}

UDialogNode* FDialogModel::ChoiceBranches() {

    /*
     * branch node: left: speak node,
     *              right:
     *                  FChoiceNode:    left:   Next choice
     *                                  right:  This choice branch
     *                                  label: choice label
     *      
     */
    const auto branchBase = NewObject<UDialogNode>(); //new UNode();
    branchBase->token = currentToken;
    Next(ETokenType::BeginBranching);
    // we're now at the spot after the choice token and expect now a speaker and text to speak.
    const auto speech = static_cast<USpeakNode*>(AssignmentOrText());
    if (!speech) {
        // ... and if it ain't, this is an epic fail
        delete branchBase;
        return nullptr;
    }
    branchBase->left = speech;
    UChoiceNode* node = NewObject<UChoiceNode>(); // new UChoiceNode();
    branchBase->right = node;
    while (currentToken.tokenType != ETokenType::EndBranching) {
        Next(ETokenType::Branch); // =>
        // now we're at the label node. Which is a Text token
        node->Init(currentToken);
        Next(ETokenType::Text);
        auto threadNode = NewObject<UDialogNode>();
        node->right = threadNode;
        bool failed = false;
        while (currentToken.tokenType != ETokenType::Branch && currentToken.tokenType != ETokenType::EndBranching) {
            // run until we hit the next branch or the end of the branching
            threadNode = RunThreadStatement(threadNode);
            if (!threadNode) {
                failed = true;
                break;
            }
        }
        // / now we oughta be at the branch

        if (failed) {
            delete branchBase;
            return nullptr;
        }

        // if we have a next branch, swap the node pointer
        if (currentToken.tokenType == ETokenType::Branch) {
            // if so, next next branch node and cycle the current node out.
            UChoiceNode* nextChoice = NewObject<UChoiceNode>(); //new UChoiceNode();
            node->left = nextChoice;
            node = nextChoice;
        }
    }

    Next(ETokenType::EndBranching);
    
    return branchBase;
}

UDialogNode* FDialogModel::Statement() {
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
            return Command();
        case ETokenType::BeginBranching:
            return ChoiceBranches();
        // todo: I'll keep this for implementing later.
        // case ETokenType::PlusPlus:
        //     break;
        // case ETokenType::MinusMinus:
        //     break;
        default:
            const FString enumType = UEnum::GetDisplayValueAsText(currentToken.tokenType).ToString();
            LOG_ERROR("Invalid %s found. Check Syntax near '%s'", *enumType, *currentToken.value);
            return nullptr;
    }
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
