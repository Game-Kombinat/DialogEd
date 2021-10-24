// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DIALOGEDEDITOR_API FDialogEdEditorModule : public IModuleInterface {
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
