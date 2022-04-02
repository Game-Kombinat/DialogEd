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
    Or
};

UCLASS()
class UBinOpNode : public UDialogNode {
    GENERATED_BODY()
public:
    UPROPERTY()
    EOperatorType opType;
    UBinOpNode();
    UBinOpNode(FParsedToken t, UDialogNode* inLeft, UDialogNode* inRight);

    void Init(FParsedToken t, UDialogNode* inLeft, UDialogNode* inRight);
};

