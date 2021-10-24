// Copyright (c) Game Kombinat


#include "StoryRunner.h"

#include "ActorRegister.h"
#include "DialogueCommandRegister.h"
#include "PreparedCommand.h"
#include "StoryThread.h"


// Sets default values for this component's properties
UStoryRunner::UStoryRunner() {
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void UStoryRunner::BeginPlay() {
    Super::BeginPlay();

    // ...
    
}


// Called every frame
void UStoryRunner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (!currentThread) {
        SetComponentTickEnabled(false);
        return;
    }
    if (!currentThread->CanContinue()) {
        SetComponentTickEnabled(false);
        return;
    }
    // if no current command or current command is done:
    if (!currentCommand.HasValidSetup() || currentCommand.IsFinished()) {
        const auto rawCmd = currentThread->GetNext();
        UDialogueCommand* command = commandRegister->GetCommand(rawCmd.commandName);
        if (!command) {
            LOG_ERROR("Unmapped command: %s", *(rawCmd.commandName));
            SetComponentTickEnabled(false);
            return;
        }
        UDialogueActor* diagActor = actorRegister->GetActorForTag(rawCmd.targetActor);
        if (!diagActor) {
            LOG_ERROR("Unmapped actor name: %s", *(rawCmd.targetActor));
            SetComponentTickEnabled(false);
            return;
        }
        
        // need actor register to get actor from names
        // need command register to get command for name
        // Then we can create a new prepared command: 
        currentCommand = FPreparedCommand(command, rawCmd.argumentList, diagActor);
        // and execute it.
        if (currentCommand.Verify()) {
            currentCommand.Run();
        }
        else {
            // todo: be nice if we get some diagnostics here as to what exactly went sideways with Verify.
            LOG_ERROR("Command did not verify.");
        }
    }
}

void UStoryRunner::StartNewStoryThread(UStoryThread* story) {
    if (currentThread && currentThread->CanContinue()) {
        LOG_ERROR("Attempted to override already running story thread.");
        return;
    }
    currentThread = story;
    currentThread->ResetThread();
    // we could reset the currentCommand here but in case it's actually still running
    // it's better to leave it until it's finished.
    SetComponentTickEnabled(true);
}

