// Copyright (c) Game Kombinat


#include "StoryAsset.h"
#include "EditorFramework/AssetImportData.h"
#include "Parser/Tree/ThreadNode.h"

// #if WITH_EDITOR
void UStoryAsset::PostInitProperties() {
    UObject::PostInitProperties();
    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        assetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
    }
}
// #endif

void UStoryAsset::AddStoryThread(UThreadNode* storyThread) {
    threads.Add(storyThread);
}

UThreadNode* UStoryAsset::GetStoryThread(const FString& storyThreadName) {
    for (const auto t : threads) {
        if (t->threadName == storyThreadName) {
            return t;
        }
    }
    return nullptr;
}