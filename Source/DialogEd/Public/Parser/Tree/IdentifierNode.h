#pragma once
#include "DialogNode.h"
#include "IdentifierNode.generated.h"

UCLASS()
class DIALOGED_API UIdentifierNode : public UDialogNode {
    GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere)
    FString identifierLabel;
    UIdentifierNode();
    explicit UIdentifierNode(FParsedToken t, UIdentifierNode* identifierType = nullptr);

    void Init(FParsedToken t, UIdentifierNode* identifierType = nullptr);

};
