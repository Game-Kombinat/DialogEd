#pragma once
#include "Tree/ThreadNode.h"

namespace DialogEd {
    class FIdentifierNode;
}

class FDialogModel {
protected:
    TArray<DialogEd::FNode*> threads;
    TArray<FParsedToken> tokenStack;
    FParsedToken lastToken;
    FParsedToken currentToken;
    FParsedToken nextToken;
public:
    FDialogModel(TArray<FParsedToken>);

    // Begins the process of generating the ast
    void Make();

    // advances to the next token on the stack rotating previous and next token entries
    void Next(ETokenType consume);
    
    // Start a new thread. everything else must be within a thread
    DialogEd::FNode* BeginThread();

    // a mathematical operation with 2 operands.
    DialogEd::FNode* BinaryOperation();

    // Choice Node containing one child to the right, the first branch
    DialogEd::FNode* BeginChoices();

    // Branch node containing next branch of choice to the right and branch content to the left
    DialogEd::FNode* Branch();

    // Begins construction of node tree based on an Identifier
    DialogEd::FNode* Identifier();

    // creates node tree for mathematical expressions
    DialogEd::FNode* LiteralOrIdentifier();

    DialogEd::FNode* MathExpression();
    DialogEd::FNode* LogicExpression();
    
    DialogEd::FNode* Text();
    DialogEd::FNode* Assignment();

    // returns terminal node after full assignment or text expressions
    DialogEd::FNode* AssignmentOrText();
    DialogEd::FNode* RunThreadStatements(DialogEd::FNode* node);

    DialogEd::FNode* If();

    DialogEd::FNode* Command();
    
    // Returns any of the valid top level nodes (if, branch, speak etc)
    DialogEd::FNode* Statement();

    bool CurrentTokenIsBinOp() const;

    bool CurrentTokenIsComparison() const;
    
    bool CurrentTokenIsLogicOp() const;

    bool NextTokenIsBinOp() const;

    bool NextTokenIsComparison() const;

    bool NextTokenIsLogicOp() const;
    
    
    
};
