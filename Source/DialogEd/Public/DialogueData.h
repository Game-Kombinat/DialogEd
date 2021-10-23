#pragma once
#include "DialogueActor.h"

#include "DialogueData.generated.h"

USTRUCT(BlueprintType)
struct FDialogueData {
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UDialogueActor* speaker;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FColor textBaseColor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString message;

    FDialogueData() {
        speaker = nullptr;
        textBaseColor = FColor::White;
        message = ""; 
    }
    FDialogueData(UDialogueActor* actor, const FString message) {
        this->message = message;
        speaker = actor;
        textBaseColor = actor->GetBaseTextColor();
    }
};
