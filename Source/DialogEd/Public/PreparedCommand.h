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
    FString arguments;
    TArray<FString> argumentList;
    class UDialogueActor* targetActor;
public:
    FPreparedCommand();
    FPreparedCommand(UDialogueCommand* command, FString args, UDialogueActor* targetActor);
    ~FPreparedCommand();
    bool HasValidSetup() const;
    bool Verify() const;
    bool IsFinished() const;
    void Run() const;
};
