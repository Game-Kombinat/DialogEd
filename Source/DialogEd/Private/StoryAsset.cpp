// Copyright (c) Game Kombinat


#include "StoryAsset.h"
#if WITH_EDITOR
#include "EditorFramework/AssetImportData.h"
#endif

#if WITH_EDITOR
void UStoryAsset::PostInitProperties() {
    UObject::PostInitProperties();
    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        assetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
    }
}
#endif

void UStoryAsset::AddStoryThread(UStoryThread* storyThread) {
    threads.Add(storyThread);
}

UStoryThread* UStoryAsset::GetStoryThread(const FString& storyThreadName) {
    for (auto t : threads) {
        if (t->GetStoryThreadName() == storyThreadName) {
            return t;
        }
    }
    return nullptr;
}
