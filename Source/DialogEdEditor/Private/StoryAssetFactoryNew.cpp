// Copyright (c) Game Kombinat


#include "StoryAssetFactoryNew.h"

#include "StoryAsset.h"

UStoryAssetFactoryNew::UStoryAssetFactoryNew() {
    SupportedClass = UStoryAsset::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

UObject* UStoryAssetFactoryNew::FactoryCreateNew(UClass* inClass, UObject* inParent, FName inName, EObjectFlags flags, UObject* context, FFeedbackContext* warn) {
    return NewObject<UStoryAsset>(inParent, inClass, inName, flags);
}

bool UStoryAssetFactoryNew::ShouldShowInNewMenu() const {
    return true;
}
