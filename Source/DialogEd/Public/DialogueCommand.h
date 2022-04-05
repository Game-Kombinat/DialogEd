// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "StoryThread.h"
#include "Kismet/GameplayStatics.h"
#include "DialogueCommand.generated.h"


/**
 * These are instantiated at a central place. The UDialogueCommandRegister.
 */
UCLASS(Abstract, Transient)
class DIALOGED_API UDialogueCommand : public UObject {
    GENERATED_BODY()
protected:
    
    UPROPERTY()
    UWorld* activeWorld;
    
public:
    /** Set the world to get all the uobjects a command may need */
    void SetWorld(UWorld* world);

    void SetStoryThread(UStoryThread* storyThread);

    virtual void Execute(TArray<class UBinOpNode*> args, class UStoryRunner* runner) PURE_VIRTUAL(UDialogueCommand::Execute);

    /**
     * Return true, when this command is done executing.
     */
    virtual bool IsFinished() PURE_VIRTUAL(UDialogueCommand::IsFinished, return true;);

    /**
     * Called after IsFinished returns true to allow for a final clean up and possible releasing of resources.
     */
    virtual void Cleanup() PURE_VIRTUAL(UDialogueCommand::Cleanup);
};
