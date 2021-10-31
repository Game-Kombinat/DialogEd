// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "DialogueCommand.h"
#include "UObject/Object.h"
#include "ChoiceCommand.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGED_API UChoiceCommand : public UDialogueCommand {
    GENERATED_BODY()
    bool choiceReceived;
    int playerChoice;
public:
    UChoiceCommand();
    virtual int MinArguments() override { return 1; } //  at least one word (we expect actor argument is removed at this point)
    virtual int MaxArguments() override { return 4; } // each arg is a choice
    
    virtual void Execute(class UDialogueActor* target, TArray<FString> args) override;
    virtual void Execute(class UDialogueActor* target, FString arg) override;
    virtual bool IsFinished() override;
    virtual void Cleanup() override;

    void ReceiveChoice(int choice);

};
