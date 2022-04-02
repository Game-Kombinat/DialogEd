#pragma once
#include "DialogNode.h"
#include "NumberNode.generated.h"

UCLASS()
class UNumberNode : public UDialogNode {
    GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere)
    int value;
    UNumberNode();
    UNumberNode(FParsedToken token);

    void Init(FParsedToken t);
};
