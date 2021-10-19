// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "DialogueCommand.generated.h"


UCLASS(Abstract)
class DIALOGED_API UDialogueCommand : public UObject{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /** Minimum number of arguments this requires. Return negative values if there is no minimum. */
    int MinArguments();
    /** Maximum number of arguments this requires. Return negative values for unlimited number of arguments. */
    int MaxArguments();

    /** Get the type of the actor that is expected as argument. May return nullptr if no actor is required. */
    UClass* TargetActorType();
    
    /** Executes the logic of this command */
    void Execute(AActor* target,  TArray<FString> args) PURE_VIRTUAL();
};
