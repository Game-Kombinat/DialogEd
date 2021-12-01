// Copyright (c) Game Kombinat


#include "StoryRunner.h"

#include "ActorRegister.h"
#include "DialogueCommandRegister.h"
#include "Logging.h"
#include "PreparedCommand.h"
#include "StoryAsset.h"
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
    FLatentActionManager::OnLatentActionsChanged().AddUObject(this, &UStoryRunner::Observe);
    // FLatentActionManager::OnLatentActionsChanged().AddUFunction(this, FName("Observe"));
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
        if (a) {
            actorsInActiveThread.Add(a);
        }
    }
    messageManager->SetRelevantActors(actorsInActiveThread);
    messageManager->SetDataContext(dataContext);
}

void UStoryRunner::Observe(UObject* obj, ELatentActionChangeType changeType) {
    if (obj) {
        LOG_INFO("LAM changed for obj %s, type %i", *obj->GetName(), static_cast<int>(changeType));
    }
    else {
        LOG_INFO("LAM changed for null obj, type %i", static_cast<int>(changeType));
    }
}

void UStoryRunner::CountRan(const UStoryThread* thread) const {
    const FString ranKey = FString::Format(TEXT("{0}__ran"), {thread->GetStoryThreadName()});
    int current = dataContext->GetValue(ranKey);
    if (current < 0) {
        current = 0;
    }
    
    dataContext->ForceSetValue(ranKey, current + 1);
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
        if (onFinished.IsBound()) {
            onFinished.Broadcast();
        }
        LOG_INFO("All threads done. Suspending StoryRunner.")
        return;
    }
    
    const auto currentThread = threadStack[threadStack.Num() - 1];
    // if we have no thread or the current thread ended and the last command is finished
    if (!currentThread || (!currentThread->CanContinue() && !currentThread->IsRunning())) {
        LOG_INFO("Thread done: %s", *currentThread->GetStoryThreadName())
        CountRan(currentThread);
        threadStack.Pop();
        return;
    }

    // This actually was just to give the fucking slate rendering time for a tick
    // to draw the god damn messaging system so it has a size.
    // But it turned out to be a useful step for many things.
    if (currentThread && !currentThread->IsPrimed()) {
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
            if (!thread) {
                LOG_ERROR("Branching target %s is invalid in thread %s!", *bt, *storyAsset->GetName());
                return;
            }
            thread->ResetStoryThread();
            threadStack.Push(thread);
            LOG_INFO("branching into %s", *bt);
            return;
        }
        messageManager->SetStoryThread(currentThread);
        const TSharedPtr<FParsedCommand> rawCmd = MakeShareable<FParsedCommand>(new FParsedCommand(currentThread->GetNext()));
        UDialogueCommand* command = commandRegister->GetCommand(rawCmd->commandName);
        if (!command) {
            LOG_ERROR("Unmapped command: %s", *(rawCmd->commandName));
            SetComponentTickEnabled(false);
            return;
        }
        
        // todo: would be better if that happens only once when instantiating the command first time
        command->SetStoryRunner(this);
        command->SetWorld(GetWorld());

        command->SetPlayerController(instigatorController);
        command->SetStoryThread(currentThread);

        FPreparedCommand currentCommand = FPreparedCommand(command, rawCmd);
        // and execute it.
        if (currentThread->RunCommand(currentCommand)) {
            LOG_INFO("Running new command: %s", *rawCmd->commandName);
        }
        else {
            // todo: be nice if we get some diagnostics here as to what exactly went sideways with Verify.
            LOG_ERROR("Command did not verify: %s", *rawCmd->commandName);
        }
    }
}

UGameDataContext* UStoryRunner::GetDataContext() {
    return dataContext;
}

void UStoryRunner::StartNewStoryThread(UStoryThread* thread, APlayerController* controller) {
    if (threadStack.Num() != 0) {
        LOG_ERROR("Cannot start a new thread while one is already running.");
        return;
    }

    if (!controller) {
        LOG_ERROR("Controller is null when starting new story!");
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
    if (!asset) {
        LOG_ERROR("No story asset given to run a thread from!");
        return;
    }
    UStoryThread* thread = asset->GetStoryThread(threadName);
    if (thread) {
        StartNewStoryThread(thread, controller);
    }
}

bool UStoryRunner::IsRunning() const {
    return threadStack.Num() > 0;
}

// void UStoryRunner::StartStory(const UObject* worldContext, UStoryRunner* runner, UStoryAsset* asset, FString threadName, APlayerController* controller, FLatentActionInfo LatentInfo) {
//     auto world = worldContext->GetWorld();
//     if (world) {
//         auto lam = world->GetLatentActionManager();
//         if (lam.FindExistingAction<FStoryRunnerDelayedAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr) {
//             // runner->StartThreadFromAsset(asset, threadName, controller);
//             lam.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FStoryRunnerDelayedAction(runner, latentInfo));
//         }
//     }
//     else {
//         LOG_ERROR("No World")
//     }
// }

// void UStoryRunner::StartStory(UStoryAsset* asset, FString threadName, APlayerController* controller, FLatentActionInfo latentInfo) {
//     auto world = GetWorld();
//     if (world) {
//         
//         auto lam = world->GetLatentActionManager();
//         if (lam.FindExistingAction<FStoryRunnerDelayedAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr) {
//             StartThreadFromAsset(asset, threadName, controller);
//             lam.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FStoryRunnerDelayedAction(this, latentInfo));
//         }
//     }
//     else {
//         LOG_ERROR("No World")
//     }
// }

UMessageManager* UStoryRunner::GetMessageManager() const {
    return messageManager;
}

UDialogueActor* UStoryRunner::GetDialogueActor(const FString& nameOrTag) const {
    return actorRegister->GetActorForTag(nameOrTag);
}

