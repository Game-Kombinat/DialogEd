﻿// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "MessageManager.h"
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
    UMessageManager* messageManager = nullptr;

    UPROPERTY()
    UWorld* activeWorld;

    UPROPERTY()
    APlayerController* controller;

    UPROPERTY()
    UStoryThread* myThread;

    UPROPERTY()
    TArray<FDialogueBranchId> branches;

    UClass* targetActorClass = nullptr;
public:
    /** Minimum number of arguments this requires. Return negative values if there is no minimum. */
    virtual int MinArguments() { return 1; }
    /** Maximum number of arguments this requires. Return negative values for unlimited number of arguments. */
    virtual int MaxArguments() { return 1; }

    /** Get the type of the actor that is expected as argument. May return nullptr if no actor is required. */
    UClass* TargetActorType() const;

    // rider may show these are not implemented due to a b_ug somewhere somehow but they are all implemented.
    /** Pass on the message manager instance to this command. */
    void SetMessageManager(UMessageManager* msgManager);

    /** Set the world to get all the uobjects a command may need */
    void SetWorld(UWorld* world);

    void SetPlayerController(APlayerController* playerController);

    void SetStoryThread(UStoryThread* storyThread);

    void SetBranches(const TArray<FDialogueBranchId> dialogueBranches);

    /**
     * Executes the logic of this command. This can be a latent action and before fetching the next one from the stack,
     * you need to check IsFinished() first or things will start to become awry real quick.
     */
    virtual void Execute(class UDialogueActor* target, TArray<FString> args) PURE_VIRTUAL(UDialogueCommand::Execute);

    virtual void Execute(class UDialogueActor* target, FString arg) PURE_VIRTUAL(UDialogueCommand::Execute);

    /**
     * Return true, when this command is done executing.
     */
    virtual bool IsFinished() PURE_VIRTUAL(UDialogueCommand::IsFinished, return true;);

    /**
     * Called after IsFinished returns true to allow for a final clean up and possible releasing of resources.
     */
    virtual void Cleanup() PURE_VIRTUAL(UDialogueCommand::Cleanup);

    /** Override and return true if you want the arguments (sans actor name) to be passed as pre-split string list. */
    virtual bool WantsArgumentsAsList() { return false; }
};
