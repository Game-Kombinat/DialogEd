// Copyright (c) Game Kombinat


#include "StoryAsset.h"
#if WITH_EDITOR
#include "EditorFramework/AssetImportData.h"
#endif

FStoryThread* UStoryAsset::GetThread(FString threadName) {
    return nullptr;
}
#if WITH_EDITOR
void UStoryAsset::PostInitProperties() {
    UObject::PostInitProperties();
    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        assetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
    }
}
#endif

void UStoryAsset::SetThreads(const TArray<FStoryThread> newThreads) {
    threads = newThreads;
}
