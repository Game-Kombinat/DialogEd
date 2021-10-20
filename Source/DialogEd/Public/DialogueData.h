#pragma once

#include "DialogueData.generated.h"

USTRUCT(BlueprintType)
struct FDialogueData {
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString speaker;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FColor textBaseColor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString message;

    FDialogueData() {
        speaker = "";
        textBaseColor = FColor::White;
        message = ""; 
    }
    FDialogueData(const FString name, const FColor color, const FString message)
    : speaker(name), textBaseColor(color), message(message) {
    }
};
