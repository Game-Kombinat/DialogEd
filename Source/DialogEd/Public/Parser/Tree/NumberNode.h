#pragma once
#include "DialogNode.h"
#include "NumberNode.generated.h"

UCLASS()
class DIALOGED_API UNumberNode : public UDialogNode {
    GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere)
    int value;
    UNumberNode();
    UNumberNode(FParsedToken token);

    void Init(FParsedToken t);
};
