#pragma once
#include "DialogueCommand.h"

/**
 * Ties together script command names to code that executes.
 * This is supposed to be used as a base for a DataTable asset.
 * This asset will then be used during runtime to work the execution stack.
 */
USTRUCT(BlueprintType)
struct FCommandRelation {
    GENERATED_BODY()
    FString commandName;
    TSubclassOf<UDialogueCommand> commandClass; 
};
