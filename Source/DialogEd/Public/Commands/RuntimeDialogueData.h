#pragma once
#include "DialogueActor.h"

#include "RuntimeDialogueData.generated.h"

/**
 * This is produced by a speak command and then passed into the dialogue manager.
 */
USTRUCT(BlueprintType)
struct FRuntimeDialogueData {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDialogueActor* speaker;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FColor textBaseColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString message;

    FRuntimeDialogueData() {
        speaker = nullptr;
        textBaseColor = FColor::White;
        message = ""; 
    }
    FRuntimeDialogueData(UDialogueActor* actor, const FString message) {
        this->message = message;
        speaker = actor;
        textBaseColor = actor->GetBaseTextColor();
    }
};
