// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "StoryAssetFactoryNew.generated.h"

/**
 * 
 */
UCLASS(hidecategories=Object)
class DIALOGEDEDITOR_API UStoryAssetFactoryNew : public UFactory {
    GENERATED_BODY()
public:
    UStoryAssetFactoryNew();
    virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
    virtual bool ShouldShowInNewMenu() const override;
};
