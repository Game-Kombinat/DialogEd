// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "ActorLookupRow.generated.h"
/**
 * 
 */
USTRUCT(Blueprintable, BlueprintType)
struct DIALOGED_API FActorLookupRow : public FTableRowBase {
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AActor> type;
    FActorLookupRow() {}
    virtual ~FActorLookupRow() override {}
};
