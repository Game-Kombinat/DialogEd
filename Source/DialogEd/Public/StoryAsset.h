﻿// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "StoryThread.h"
#include "UObject/Object.h"
#include "StoryAsset.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGED_API UStoryAsset : public UObject {
    GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FStoryThread> threads;

    FStoryThread* GetThread(FString threadName);
public:
    void SetThreads(TArray<FStoryThread> threads);
};
