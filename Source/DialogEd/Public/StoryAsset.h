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
    TArray<class UThreadNode*> threads;

public:
// #if WITH_EDITOR
    UPROPERTY()
    class UAssetImportData* assetImportData;
    virtual void PostInitProperties() override;
// #endif
    
    void AddStoryThread(UThreadNode* storyThread);
    class UThreadNode* GetStoryThread(const FString& storyThreadName);
};
