// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "StoryThread.h"
#include "UObject/Object.h"
#include "StoryAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class DIALOGED_API UStoryAsset : public UObject {
    GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<UStoryThread*> threads;
    
    /** Used to facilitate branching. Sub Threads can be single commands or a list of commands. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TMap<FString, UStoryThread*> subThreads;

public:
#if WITH_EDITOR
    UPROPERTY()
    UAssetImportData* assetImportData;
    virtual void PostInitProperties() override;
#endif
    
    void AddStoryThread(UStoryThread* storyThread);
    UStoryThread* GetStoryThread(const FString& storyThreadName);
    void AddSubThread(const FString& string, UStoryThread* storyThread);

    UStoryThread* GetSubThread(const FString address);
};
