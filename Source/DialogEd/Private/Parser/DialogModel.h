#pragma once
#include "ParsedToken.h"

namespace DialogEd {
    class FIdentifierNode;
    class FNode;
    
}

class FDialogModel {

protected:
    TArray<DialogEd::FNode*> threads;
    TArray<FParsedToken> tokenStack;
    FParsedToken lastToken;
    FParsedToken currentToken;
    FParsedToken nextToken;
public:
    explicit FDialogModel(const TArray<FParsedToken> tokens);
    ~FDialogModel();

    TArray<DialogEd::FNode*> GetData() { return threads; }
    // Begins the process of generating the ast
    void Make();

    FString ToString();

    static void TreeToString(DialogEd::FNode* root, FString& buffer);

    // advances to the next token on the stack rotating previous and next token entries
    void Next(ETokenType consume);
    
    // Start a new thread. everything else must be within a thread
    DialogEd::FNode* BeginThread();

    // Begins construction of node tree based on an Identifier
    DialogEd::FNode* Identifier();

    // creates node tree for mathematical expressions
    DialogEd::FNode* LiteralOrIdentifier();

    DialogEd::FNode* MathExpression();
    DialogEd::FNode* LogicExpression();
    
    DialogEd::FNode* Text(DialogEd::FNode* lhsIdentifier);
    DialogEd::FNode* Assignment(DialogEd::FNode* lhsIdentifier);

    // returns terminal node after full assignment or text expressions
    DialogEd::FNode* AssignmentOrText();
    DialogEd::FNode* RunThreadStatement(DialogEd::FNode* node);

    DialogEd::FNode* If();

    DialogEd::FNode* Command();

    DialogEd::FNode* ChoiceBranches();
    
    // Returns any of the valid top level nodes (if, branch, speak etc)
    DialogEd::FNode* Statement();

    bool CurrentTokenIsBinOp() const;

    bool CurrentTokenIsComparison() const;
    
    bool CurrentTokenIsLogicOp() const;

    bool NextTokenIsBinOp() const;

    bool NextTokenIsComparison() const;

    bool NextTokenIsLogicOp() const;
    
    
    
};
