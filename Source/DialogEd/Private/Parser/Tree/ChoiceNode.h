#pragma once
#include "DialogNode.h"
#include "ChoiceNode.generated.h"
UCLASS()
class UChoiceNode : public UDialogNode {
    GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere)
    FString choiceLabel;
    explicit UChoiceNode(FParsedToken t);
    UChoiceNode();
    void Init(FParsedToken t);
};