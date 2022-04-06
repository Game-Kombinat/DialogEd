#pragma once
#include "ParsedToken.h"

class UIdentifierNode;
class UDialogNode;

class DIALOGED_API FDialogModel {

protected:
    TArray<UDialogNode*> threads;
    TArray<FParsedToken> tokenStack;
    FParsedToken lastToken;
    FParsedToken currentToken;
    FParsedToken nextToken;
    class UStoryAsset* owner;
public:
    explicit FDialogModel(const TArray<FParsedToken> tokens, class UStoryAsset* ownerAsset);
    // ~FDialogModel();

    TArray<UDialogNode*> GetData() { return threads; }
    // Begins the process of generating the ast
    void Make();

    FString ToString();

    static void TreeToString(UDialogNode* root, FString& buffer);

    // advances to the next token on the stack rotating previous and next token entries
    void Next(ETokenType consume);
    
    // Start a new thread. everything else must be within a thread
    UDialogNode* BeginThread();

    // Begins construction of node tree based on an Identifier
    UDialogNode* Identifier();

    // creates node tree for mathematical expressions
    UDialogNode* LiteralOrIdentifier();

    UDialogNode* MathExpression();
    UDialogNode* LogicExpression();
    
    UDialogNode* Text(UDialogNode* lhsIdentifier);
    UDialogNode* Assignment(UDialogNode* lhsIdentifier);

    // returns terminal node after full assignment or text expressions
    UDialogNode* AssignmentOrText();
    UDialogNode* RunThreadStatement(UDialogNode* node);

    UDialogNode* If();

    UDialogNode* Command();

    UDialogNode* ChoiceBranches();
    
    // Returns any of the valid top level nodes (if, branch, speak etc)
    UDialogNode* Statement();

    bool CurrentTokenIsBinOp() const;

    bool CurrentTokenIsComparison() const;
    
    bool CurrentTokenIsLogicOp() const;

    bool NextTokenIsBinOp() const;

    bool NextTokenIsComparison() const;

    bool NextTokenIsLogicOp() const;
    
    
    
};
