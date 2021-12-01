﻿// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "ParsedCommand.h"

/**
 * This is a ready-to-execute piece of command that sits on the execution stack
 * or the wait queue and can be fired at will anytime now.
 */
class DIALOGED_API FPreparedCommand {
    TWeakObjectPtr<class UDialogueCommand> logic;
    TSharedPtr<FParsedCommand> command;
public:
    FPreparedCommand();
    FPreparedCommand(TWeakObjectPtr<class UDialogueCommand> command, TSharedPtr<FParsedCommand> rawCmd);
    ~FPreparedCommand();
    bool HasValidSetup() const;
    bool Verify() const;
    bool IsFinished() const;
    void Run() const;
    void Cleanup();
};
