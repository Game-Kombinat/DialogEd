#pragma once
#include "DialogNode.h"
#include "BinOpNode.generated.h"

UENUM()
enum class EOperatorType : uint8 {
    NoOp,
    Add,
    Sub,
    Mult,
    Div,
    Less,
    Greater,
    LessOrEqual,
    GreaterOrEqual,
    Equal,
    NotEqual,
    And,
    Or,
    Assignment
};

UCLASS()
class DIALOGED_API UBinOpNode : public UDialogNode {
    GENERATED_BODY()
public:
    
    UPROPERTY(VisibleAnywhere)
    EOperatorType opType;
    UBinOpNode();
    UBinOpNode(FParsedToken t, UDialogNode* inLeft, UDialogNode* inRight);

    void Init(FParsedToken t, UDialogNode* inLeft, UDialogNode* inRight);
    int Evaluate(class UStoryRunner* storyRunner) const;

    int GetLeftValue(UStoryRunner* runner) const;
    
    int GetRightValue(UStoryRunner* runner) const;

    int Assign(UStoryRunner* runner) const;

    FString GetLeftIdentifier() const;
    FString GetLeftType() const;

    static int GetNodeValue(UStoryRunner* runner, UDialogNode* node);
    
};

