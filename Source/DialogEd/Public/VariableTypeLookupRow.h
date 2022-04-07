// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "VariableTypeHandler.h"
#include "Engine/DataTable.h"

#include "VariableTypeLookupRow.generated.h"
/**
 * 
 */
USTRUCT(Blueprintable, BlueprintType)
struct DIALOGED_API FVariableTypeLookupRow : public FTableRowBase {
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UVariableTypeHandler> type;
    FVariableTypeLookupRow() {}
    virtual ~FVariableTypeLookupRow() override {}
};
