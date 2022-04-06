#pragma once
#include "DialogNode.h"
#include "ChoiceNode.generated.h"
UCLASS()
class DIALOGED_API UChoiceNode : public UDialogNode {
    GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere)
    FString choiceLabel;
    explicit UChoiceNode(FParsedToken t);
    UChoiceNode();
    void Init(FParsedToken t);

    TMap<int, FString> GetChoices() const;
    
    UChoiceNode* GetChoice(int choice);

    UDialogNode* GetBranch() const;
};