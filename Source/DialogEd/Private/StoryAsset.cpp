// Copyright (c) Game Kombinat


#include "StoryAsset.h"

FStoryThread* UStoryAsset::GetThread(FString threadName) {
    return nullptr;
}

void UStoryAsset::SetThreads(const TArray<FStoryThread> newThreads) {
    threads = newThreads;
}
