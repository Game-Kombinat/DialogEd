// Copyright (c) Game Kombinat


#include "StoryAssetFactory.h"

#include "Logging.h"
#include "StoryAsset.h"
#include "EditorFramework/AssetImportData.h"
#include "Misc/FileHelper.h"
#include "Parser/DialogModel.h"
#include "Parser/Tokenizer.h"
#include "Parser/Tree/ThreadNode.h"


UStoryAssetFactory::UStoryAssetFactory() {
    Formats.Add(FString(TEXT("story;Story File")));
    SupportedClass = UStoryAsset::StaticClass();
    bCreateNew = false;
    bEditorImport = true;
}

UObject* UStoryAssetFactory::FactoryCreateFile(UClass* inClass, UObject* inParent, FName inName, EObjectFlags flags, const FString& filename, const TCHAR* parms, FFeedbackContext* warn, bool& outOperationCanceled) {
    auto story = NewObject<UStoryAsset>(inParent, inClass, inName, flags);
    story->assetImportData->Update(filename);
    FTokenizer t;
    FString input;
    
    if (!FFileHelper::LoadFileToString(input, *filename)) {
        LOG_ERROR("Failed reading test story");
        outOperationCanceled = true;
        return story;
    }
    auto tokens = t.Tokenize(input);
    TArray<FString> tokenStrings;

    auto dm = MakeShared<FDialogModel>(tokens, story);
    dm->Make();
    for (auto node : dm->GetData()) {
        auto thread = static_cast<UThreadNode*>(node);
        story->AddStoryThread(thread);
    }
    
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
