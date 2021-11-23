// Copyright (c) Game Kombinat


#include "StoryRunnerHelper.h"

#include "StoryRunner.h"

UStoryRunnerHelper* UStoryRunnerHelper::StartStory(UStoryRunner* runner, UStoryAsset* asset, FString threadName, APlayerController* controller) {

    auto obj = NewObject<UStoryRunnerHelper>();
    obj->Prepare(runner, asset, threadName, controller);

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

void UStoryRunnerHelper::Prepare(UStoryRunner* runner, UStoryAsset* asset, FString threadName, APlayerController* controller) {
    storyRunner = runner;
    storyRunner->StartThreadFromAsset(asset, threadName, controller);
    storyRunner->onFinished.AddDynamic(this, &UStoryRunnerHelper::OnStoryFinished);
}

void UStoryRunnerHelper::OnStoryFinished() {
    if (storyFinished.IsBound()) {
        storyFinished.Broadcast();
        storyRunner->onFinished.RemoveDynamic(this, &UStoryRunnerHelper::OnStoryFinished);
    }
}

