// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StoryAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGEDEDITOR_API UStoryAssetFactory : public UFactory {
    GENERATED_BODY()
public:
    UStoryAssetFactory();
    virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;
};
