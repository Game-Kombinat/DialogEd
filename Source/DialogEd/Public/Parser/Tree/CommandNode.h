#pragma once
#include "DialogNode.h"
#include "CommandNode.generated.h"
UCLASS()
class DIALOGED_API UCommandNode : public UDialogNode {
    GENERATED_BODY()
public:
    UCommandNode();

    FString GetCommandName() const;

    TArray<class UBinOpNode*> GetArgs() const;
};
