#pragma once
#include "DialogueActor.h"

#include "DialogueData.generated.h"

USTRUCT(BlueprintType)
struct FDialogueData {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDialogueActor* speaker;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FColor textBaseColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
