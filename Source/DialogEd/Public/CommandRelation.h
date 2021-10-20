#pragma once
#include "DialogueCommand.h"
#include "Engine/DataTable.h"
#include "CommandRelation.generated.h"

/**
 * Ties together script command names to code that executes.
 * This is supposed to be used as a base for a DataTable asset.
 * This asset will then be used during runtime to work the execution stack.
 */
USTRUCT(BlueprintType)
struct FCommandRelation : public FTableRowBase {
    GENERATED_BODY()
    
    // could be FName but this saves some juggling later down the line.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString commandName; 

    // maps the name to an instantiatable class that implements IDialogueCommand
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UDialogueCommand> commandClass;
};
