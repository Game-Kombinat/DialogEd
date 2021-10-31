#pragma once

#include "CoreMinimal.h"
#include "DialogueBranchId.generated.h"

/**
 * Contains the data for a dialogue branch.
 * That is the branch title for display
 * and its internal branching address in the sub thread list of a DialogueThread. 
 */
USTRUCT(BlueprintType)
struct FDialogueBranchId {
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString title;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString branchId;

    FDialogueBranchId() {
    }

    FDialogueBranchId(FString inTitle, FString inBranchId) {
        title = inTitle;
        branchId = inBranchId;
    }
};
