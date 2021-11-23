// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "DialogueCommand.h"

/**
 * 
 */
class DIALOGED_API SetVarCommand : public UDialogueCommand {
public:
    SetVarCommand();
    virtual ~SetVarCommand() override;

    virtual int MinArguments() override { return 2; } //  var name and var value
    virtual int MaxArguments() override { return 2; } //  and nothing more!
    
    virtual void Execute(TSharedRef<FParsedCommand>) override;
    virtual bool IsFinished() override;
    virtual void Cleanup() override;
};
