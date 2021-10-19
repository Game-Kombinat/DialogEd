// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "DialogueCommand.h"

/**
 * This is a ready-to-execute piece of command that sits on the execution stack
 * or the wait queue and can be fired at will anytime now.
 */
class DIALOGED_API FPreparedCommand {
    UDialogueCommand* logic;
    TArray<FString> arguments;
    AActor* targetActor;
public:
    FPreparedCommand(UDialogueCommand* command, TArray<FString> args, AActor* targetActor);
    ~FPreparedCommand();
    bool Verify() const;
    void Run() const;
};
