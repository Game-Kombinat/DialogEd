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
    // arguments here denote the text spoken to formulate a question.
    // the choices are not part of the argument list (see SetBranches etc)
    virtual int MinArguments() override { return 1; }
    virtual int MaxArguments() override { return -1; }
    
    virtual void Execute(class UDialogueActor* target, TArray<FString> args) override;
    virtual void Execute(class UDialogueActor* target, FString arg) override;
    virtual bool IsFinished() override;
    virtual void Cleanup() override;
    UFUNCTION()
    void ReceiveChoice(int choice);
};
