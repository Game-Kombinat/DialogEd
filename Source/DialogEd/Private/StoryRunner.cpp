// Copyright (c) Game Kombinat


#include "StoryRunner.h"

#include "ActorRegister.h"
#include "DialogueCommandRegister.h"
#include "Logging.h"
#include "PreparedCommand.h"
#include "StoryThread.h"
#include "GameFramework/Character.h"
#include "Ui/MessagingWidget.h"


// Sets default values for this component's properties
UStoryRunner::UStoryRunner() {
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    // currentThread = emptyThread;
}


// Called when the game starts
void UStoryRunner::BeginPlay() {
    Super::BeginPlay();
    actorRegister->OnBeginPlay(GetWorld());
    commandRegister->OnBeginPlay();
    SetComponentTickEnabled(false);
}


// Called every frame
void UStoryRunner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // if we have no thread or the current thread ended and the last command is finished
    if (!currentThread || (!currentThread->CanContinue() && !currentThread->IsRunning())) {
        SetComponentTickEnabled(false);
        messageManager->messaging->RemoveFromViewport();
        // give back character controls when thread is over.
        instigatorCharacter->EnableInput(instigatorController);
        LOG_INFO("Thread ended. Suspending ticking StoryRunner.")
        return;
    }

    // This is actually just to give the fucking slate rendering time for a tick
    // to draw the god damn messaging system so it has a size.
    // There ought to be a better solution but I can find none.
    if (currentThread && !currentThread->IsPrimed()) {
        currentThread->Prime();
        return;
    }
    // if no current command or current command is done:
    if (!currentThread->IsRunning()) {
        currentThread->CleanupCommand();
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
        // todo: would be better if that happens only once when instantiating the command first time
        command->SetMessageManager(messageManager);
        command->SetWorld(GetWorld());

        command->SetPlayerController(instigatorController);
        command->SetStoryThread(currentThread);
        
        FPreparedCommand currentCommand = FPreparedCommand(command, rawCmd.argumentList, diagActor);
        // and execute it.
        if (currentThread->RunCommand(currentCommand)) {
            LOG_INFO("Running new command!");
        }
        else {
            // todo: be nice if we get some diagnostics here as to what exactly went sideways with Verify.
            LOG_ERROR("Command did not verify.");
        }
    }
}

void UStoryRunner::StartNewStoryThread(UStoryThread* story, APlayerController* controller) {
    if (currentThread && currentThread->CanContinue()) {
        LOG_ERROR("Attempted to override already running story thread.");
        return;
    }
    messageManager->SetActionName(inputAction);
    messageManager->messaging->AddToViewport();
    messageManager->messaging->ForceLayoutPrepass();
    // remove controls from instigator char and store pointers for putting back later
    const auto controlledChar = controller->GetCharacter();
    controlledChar->DisableInput(controller);
    instigatorController = controller;
    instigatorCharacter = controlledChar;
    
    currentThread = story;
    currentThread->ResetStoryThread();
    // we could reset the currentCommand here but in case it's actually still running
    // it's better to leave it until it's finished.
    SetComponentTickEnabled(true);
}

void UStoryRunner::StartThreadFromAsset(UStoryAsset* asset, FString threadName, APlayerController* controller) {
    UStoryThread* thread = asset->GetStoryThread(threadName);
    if (thread) {
        StartNewStoryThread(thread, controller);
    }
}

