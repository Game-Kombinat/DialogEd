#pragma once
#include "StoryRunner.h"
#include "Parser/ParsedToken.h"
#include "DialogNode.generated.h"

UCLASS()
class DIALOGED_API UDialogNode : public UObject {
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    FParsedToken token;
    
    UPROPERTY(EditDefaultsOnly)
    UDialogNode* left;
    
    UPROPERTY(EditDefaultsOnly)
    UDialogNode* right;
    
    UDialogNode();
    // ~UDialogNode();
    FString ToString();

    FString ToString(int indent);
    // Get the next node
    virtual UDialogNode* Next();

    virtual UDialogNode* Statements();
};
