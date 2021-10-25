// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "EditorReimportHandler.h"
#include "UObject/Object.h"
#include "StoryAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGEDEDITOR_API UStoryAssetFactory : public UFactory, public FReimportHandler {
    GENERATED_BODY()
public:
    UStoryAssetFactory();
    virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;

    // reimoprt handler
    virtual bool CanReimport(UObject* obj, TArray<FString>& outFileNames) override;
    virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
    virtual EReimportResult::Type Reimport(UObject* Obj) override;
};
