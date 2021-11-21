// Copyright (c) Game Kombinat


#include "StoryRunner.h"

#include "ActorRegister.h"
#include "DialogueCommandRegister.h"
#include "Logging.h"
#include "PreparedCommand.h"
#include "StoryThread.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
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

void UStoryRunner::EndPlay(const EEndPlayReason::Type endPlayReason) {
    // if we stop play mid execution, we need to clean this up
    if (threadStack.Num() > 0) {
        for (auto t : threadStack) {
            if (t) {
                t->CleanupCommand();
            }
        }
    }
}

void UStoryRunner::HandleActorsInThread() {
    actorsInActiveThread.Empty();
    auto currentThread = threadStack[threadStack.Num() - 1];
    if (!currentThread) {
        LOG_ERROR("Cannot handle actors in current thread - there is no current thread!");
        return;
    }
    for (const auto a : currentThread->actorsInThread) {
        auto resolvedActor = actorRegister->GetActorForTag(a);
        if (resolvedActor) {
            actorsInActiveThread.Add(resolvedActor);
        }
    }
    messageManager->SetRelevantActors(actorsInActiveThread);
    messageManager->SetDataContext(dataContext);
}


// Called every frame
void UStoryRunner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (threadStack.Num() == 0) {
        SetComponentTickEnabled(false);
        messageManager->RemoveFromViewport();
        UWidgetBlueprintLibrary::SetInputMode_GameOnly(instigatorController);
        instigatorController->SetInputMode(FInputModeGameOnly());
        // give back character controls when thread is over.
        instigatorCharacter->EnableInput(instigatorController);
        // instigatorController->SetShowMouseCursor(false);
        LOG_INFO("Thread ended. Suspending ticking StoryRunner.")
        return;
    }
    
    const auto currentThread = threadStack[threadStack.Num() - 1];
    // if we have no thread or the current thread ended and the last command is finished
    if (!currentThread || (!currentThread->CanContinue() && !currentThread->IsRunning())) {
        LOG_INFO("Thread done. Removing %s", *currentThread->GetStoryThreadName())
        threadStack.Pop();
        return;
    }

    // This actually was just to give the fucking slate rendering time for a tick
    // to draw the god damn messaging system so it has a size.
    // But it turned out to be a useful step for many things.
    if (currentThread && !currentThread->IsPrimed()) {
        LOG_INFO("Priming thread %s", *currentThread->GetStoryThreadName());
        currentThread->Prime();
        HandleActorsInThread();
        return;
    }
    // if no current command or current command is done:
    if (!currentThread->IsRunning() && currentThread->CanContinue()) {
        currentThread->CleanupCommand();
        if (currentThread->IsBranching()) {
            auto bt = currentThread->GetBranchingTarget();
            currentThread->BranchingConsumed();
            auto thread = storyAsset->GetSubThread(bt);
            thread->ResetStoryThread();
            threadStack.Push(thread);
            LOG_INFO("branching into %s", *bt);
            return;
        }
        messageManager->SetStoryThread(currentThread);
        const auto rawCmd = currentThread->GetNext();
        UDialogueCommand* command = commandRegister->GetCommand(rawCmd.commandName);
        if (!command) {
            LOG_ERROR("Unmapped command: %s", *(rawCmd.commandName));
            SetComponentTickEnabled(false);
            return;
        }
        UDialogueActor* diagActor = nullptr;
        if (rawCmd.requiresActor) {
            diagActor = actorRegister->GetActorForTag(rawCmd.targetActor);
            if (!diagActor) {
                LOG_ERROR("Unmapped actor name: %s", *(rawCmd.targetActor));
                SetComponentTickEnabled(false);
                return;
            }
        }
        
        // todo: would be better if that happens only once when instantiating the command first time
        command->SetMessageManager(messageManager);
        command->SetWorld(GetWorld());

        command->SetPlayerController(instigatorController);
        command->SetStoryThread(currentThread);
        command->SetBranches(rawCmd.branches);
        
        FPreparedCommand currentCommand = FPreparedCommand(command, rawCmd, diagActor);
        // and execute it.
        if (currentThread->RunCommand(currentCommand)) {
            LOG_INFO("Running new command: %s", *rawCmd.commandName);
        }
        else {
            // todo: be nice if we get some diagnostics here as to what exactly went sideways with Verify.
            LOG_ERROR("Command did not verify: %s", *rawCmd.commandName);
        }
    }
}

void UStoryRunner::StartNewStoryThread(UStoryThread* thread, APlayerController* controller) {
    if (threadStack.Num() != 0) {
        LOG_ERROR("Cannot start a new thread while one is already running.");
        return;
    }
    
    messageManager->AddToViewport();
    // remove controls from instigator char and store pointers for putting back later
    const auto controlledChar = controller->GetCharacter();
    UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(controller);
    controlledChar->DisableInput(controller);
    // controller->SetShowMouseCursor(true);
    instigatorController = controller;
    instigatorCharacter = controlledChar;

    threadStack.Push(thread);
    thread->ResetStoryThread();
    storyAsset = thread->GetStoryAsset();
    SetComponentTickEnabled(true);
}

void UStoryRunner::StartThreadFromAsset(UStoryAsset* asset, FString threadName, APlayerController* controller) {
    UStoryThread* thread = asset->GetStoryThread(threadName);
    if (thread) {
        StartNewStoryThread(thread, controller);
    }
}

