// Copyright (c) Game Kombinat


#include "StoryAssetFactory.h"

#include "FileParser.h"
#include "StoryAsset.h"
#include "EditorFramework/AssetImportData.h"

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
    story->assetImportData->Update(filename);
    outOperationCanceled = false;

    return story;

}

bool UStoryAssetFactory::CanReimport(UObject* obj, TArray<FString>& outFileNames) {
    UStoryAsset* storyAsset = Cast<UStoryAsset>(obj);
    if (storyAsset && storyAsset->assetImportData) {
        storyAsset->assetImportData->ExtractFilenames(outFileNames);
        return true;
    }
    return false;
}

void UStoryAssetFactory::SetReimportPaths(UObject* obj, const TArray<FString>& newReimportPaths) {
    UStoryAsset* storyAsset = Cast<UStoryAsset>(obj);
    if (storyAsset && ensure(newReimportPaths.Num() == 1)) {
        storyAsset->assetImportData->UpdateFilenameOnly(newReimportPaths[0]);
    }
}

EReimportResult::Type UStoryAssetFactory::Reimport(UObject* obj) {
    UStoryAsset* storyAsset = Cast<UStoryAsset>(obj);
    if (storyAsset != nullptr) {
        if (storyAsset->assetImportData) {
            bool bOperationCancelled = false;
            if (ImportObject(storyAsset->GetClass(), storyAsset->GetOuter(), *storyAsset->GetName(), RF_Public | RF_Standalone, storyAsset->assetImportData->GetFirstFilename(), nullptr, bOperationCancelled) != nullptr) {
                return EReimportResult::Succeeded;
            }
        }
    }

    return EReimportResult::Failed;
}
