#pragma once
#include "DialogNode.h"
#include "IdentifierNode.generated.h"

UCLASS()
class DIALOGED_API UIdentifierNode : public UDialogNode {
    GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere)
    FString identifierLabel;
    // Used to uniquely identify this identifier.
    // Important, for one, for magic variables.
    UPROPERTY(VisibleAnywhere)
    FGuid guid;
    
    UIdentifierNode();
    explicit UIdentifierNode(FParsedToken t, UIdentifierNode* identifierType = nullptr);

    void Init(FParsedToken t, UIdentifierNode* identifierType = nullptr);
    FString GetIdentifierType() const;

    FGuid& GetGuid();
};
