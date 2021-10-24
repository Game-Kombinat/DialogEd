// Copyright (c) Game Kombinat


#include "StoryAssetFactory.h"

#include "FileParser.h"
#include "StoryAsset.h"

UStoryAssetFactory::UStoryAssetFactory() {
    Formats.Add(FString(TEXT("story;Story File")));
    SupportedClass = UStoryAsset::StaticClass();
    bCreateNew = false;
    bEditorImport = true;
}

UObject* UStoryAssetFactory::FactoryCreateFile(UClass* inClass, UObject* inParent, FName inName, EObjectFlags flags, const FString& filename, const TCHAR* parms, FFeedbackContext* warn, bool& outOperationCanceled) {
    FileParser p(filename);
    auto threads = p.Parse();

    auto story = NewObject<UStoryAsset>(inParent, inClass, inName, flags);
    story->SetThreads(threads);
    
    outOperationCanceled = false;

    return story;
    
}
