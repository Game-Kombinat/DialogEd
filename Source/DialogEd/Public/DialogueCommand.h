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
    class UStoryRunner* storyRunner = nullptr;

    UPROPERTY()
    UWorld* activeWorld;

    UPROPERTY()
    APlayerController* controller;

    UPROPERTY()
    UStoryThread* myThread;

public:
    /** Minimum number of arguments this requires. Return negative values if there is no minimum. */
    virtual int MinArguments() { return 1; }
    /** Maximum number of arguments this requires. Return negative values for unlimited number of arguments. */
    virtual int MaxArguments() { return 1; }

    // rider may show these are not implemented due to a b_ug somewhere somehow but they are all implemented.
    /** Pass on the message manager instance to this command. */
    void SetStoryRunner(UStoryRunner* runner);

    /** Set the world to get all the uobjects a command may need */
    void SetWorld(UWorld* world);

    void SetPlayerController(APlayerController* playerController);

    void SetStoryThread(UStoryThread* storyThread);

    virtual void Execute(TSharedRef<FParsedCommand> cmd) PURE_VIRTUAL(UDialogueCommand::Execute);

    /**
     * Return true, when this command is done executing.
     */
    virtual bool IsFinished() PURE_VIRTUAL(UDialogueCommand::IsFinished, return true;);

    /**
     * Called after IsFinished returns true to allow for a final clean up and possible releasing of resources.
     */
    virtual void Cleanup() PURE_VIRTUAL(UDialogueCommand::Cleanup);
};
