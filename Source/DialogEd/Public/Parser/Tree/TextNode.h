#pragma once
#include "DialogNode.h"
#include "TextNode.generated.h"

UCLASS()
class DIALOGED_API UTextNode : public UDialogNode {
    GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere)
    FString value;
    UTextNode();
    explicit UTextNode(FParsedToken t, FString inValue);

    void Init(FParsedToken t, FString inValue);
};
