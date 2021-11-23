// Copyright (c) Game Kombinat

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

    virtual int MinArguments() override { return 2; } //  at least the speaker and one word
    virtual int MaxArguments() override { return -1; } //  we can have an unspecified amount of words.
    
    virtual void Execute(TSharedRef<FParsedCommand> cmd) override;
    virtual bool IsFinished() override;
    virtual void Cleanup() override;

};
