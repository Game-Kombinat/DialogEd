// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "StoryRunnerHelper.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResponsDelegate);
/**
 * 
 */
UCLASS(meta=(HideThen=true))
class DIALOGED_API UStoryRunnerHelper : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

    UPROPERTY(BlueprintAssignable)
    FResponsDelegate storyFinished;

protected:
    UPROPERTY()
    class UStoryRunner* storyRunner;

    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
    static UStoryRunnerHelper* StartStory(class UStoryRunner* runner, class UStoryAsset* asset, FString threadName, APlayerController* controller);

    void Prepare(class UStoryRunner* runner, class UStoryAsset* asset, FString threadName, APlayerController* controller);

    UFUNCTION()
    void OnStoryFinished();
    
    // UFUNCTION(BlueprintCallable, meta=(Latent, WorldContext="worldContext", LatentInfo="LatentInfo"))
    // static void StartStory(const UObject* worldContext, class UStoryRunner* runner, class UStoryAsset* asset, FString threadName, APlayerController* controller, struct FLatentActionInfo latentInfo);
};
