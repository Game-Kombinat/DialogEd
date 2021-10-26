// Copyright (c) Game Kombinat

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
    TArray<UStoryThread*> threads;

public:
#if WITH_EDITOR
    UPROPERTY()
    UAssetImportData* assetImportData;
    virtual void PostInitProperties() override;
#endif
    
    void AddStoryThread(UStoryThread* storyThread);
    UStoryThread* GetStoryThread(const FString& storyThreadName);
};
