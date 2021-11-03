﻿// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "DialogueCommand.h"
#include "SpeakCommand.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGED_API USpeakCommand : public UDialogueCommand {
    GENERATED_BODY()
public:
    USpeakCommand();
    virtual ~USpeakCommand() override;

    virtual int MinArguments() override { return 1; } //  at least one word (we expect actor argument is removed at this point)
    virtual int MaxArguments() override { return -1; } //  we can have an unspecified amount of words.
    
    virtual void Execute(class UDialogueActor* target, TArray<FString> args) override;
    virtual void Execute(class UDialogueActor* target, FString arg) override;
    virtual bool IsFinished() override;
    virtual void Cleanup() override;

};
