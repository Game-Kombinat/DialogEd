// Copyright (c) Game Kombinat


#include "StoryRunnerHelper.h"

#include "Logging.h"
#include "StoryRunner.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

UStoryRunnerHelper* UStoryRunnerHelper::StartStory(UObject* worldContext, UStoryAsset* asset, FString threadName) {
    UStoryRunner* runner = Cast<UStoryRunner>(worldContext->GetWorld()->GetAuthGameMode()->FindComponentByClass(UStoryRunner::StaticClass()));
    if (!runner) {
        LOG_ERROR("Could not find a story runner on the game mode. It needs to be on the game mode!");
        return nullptr;
    }
    const auto obj = NewObject<UStoryRunnerHelper>();
    obj->Prepare(runner, asset, threadName);

    return obj;
    // auto world = runner->GetWorld();
    // if (world) {
    //     if (latentInfo.UUID == 0) {
    //         latentInfo.UUID = 512;
    //     }
    //     auto lam = world->GetLatentActionManager();
    //     if (lam.FindExistingAction<FStoryRunnerDelayedAction>(latentInfo.CallbackTarget, latentInfo.UUID) == nullptr) {
    //         
    //         // runner->StartThreadFromAsset(asset, threadName, controller);
    //         lam.AddNewAction(latentInfo.CallbackTarget, latentInfo.UUID, new FStoryRunnerDelayedAction(runner, latentInfo));
    //     }
    // }
    // else {
    //     LOG_ERROR("No World");
    // }
}

void UStoryRunnerHelper::Prepare(UStoryRunner* runner, UStoryAsset* asset, FString threadName) {
    storyRunner = runner;
    if (!runner) {
        LOG_ERROR("No runner to run a story");
        return;
    }
    storyRunner->StartThreadFromAsset(asset, threadName);
}

void UStoryRunnerHelper::OnStoryFinished() {
    if (storyFinished.IsBound()) {
        storyFinished.Broadcast();
    }
}

