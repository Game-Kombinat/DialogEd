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
    TArray<class UDialogNode*> threads;

public:
// #if WITH_EDITOR
    UPROPERTY()
    class UAssetImportData* assetImportData;
    virtual void PostInitProperties() override;
// #endif
    
    void AddStoryThread(UDialogNode* storyThread);
    UStoryThread* GetStoryThread(const FString& storyThreadName);
    void AddSubThread(const FString& string, UStoryThread* storyThread);

    UStoryThread* GetSubThread(const FString address);
};
