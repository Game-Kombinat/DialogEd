// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "DialogueCommand.h"
#include "SetVarCommand.generated.h"
/**
 * 
 */
UCLASS()
class DIALOGED_API USetVarCommand : public UDialogueCommand {
    GENERATED_BODY()
public:
    USetVarCommand();
    virtual ~USetVarCommand() override;

    virtual int MinArguments() override { return 2; } //  var name and var value
    virtual int MaxArguments() override { return 2; } //  and nothing more!
    
    virtual void Execute(TSharedPtr<FParsedCommand>) override;
    virtual bool IsFinished() override;
    virtual void Cleanup() override;
};
