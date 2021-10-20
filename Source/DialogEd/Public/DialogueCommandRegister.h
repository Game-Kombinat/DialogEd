// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "CommandRelation.h"
#include "DialogueCommandRegister.generated.h"

/**
 * Register asset that is used to fetch actual code for dialogue instructions.
 */
UCLASS()
class DIALOGED_API UDialogueCommandRegister : public UDataAsset {
    GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FCommandRelation> commandMap;

    // each command has a single instance that is kept here during runtime.
    UPROPERTY(Transient)
    TMap<FString, UDialogueCommand*> instantiatedCommands;

    UClass* FindClassForName(const FString& name);
    
public:
    /** Get command for name. May return nullptr on unmapped commands! */
    UDialogueCommand* GetCommand(FString name);

    /** Call some initialisation code on begin play. */
    void OnBeginPlay();
};
