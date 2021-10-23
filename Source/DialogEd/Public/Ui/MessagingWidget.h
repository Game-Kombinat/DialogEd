// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "SpeechBubbleWidget.h"
#include "UObject/Object.h"
#include "MessagingWidget.generated.h"

/**
 * Handles the rendering of speech bubbles.
 */
UCLASS()
class DIALOGED_API UMessagingWidget : public UUserWidget {
    GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<USpeechBubbleWidget> bubbleType;
private:
    UPROPERTY()
    TArray<USpeechBubbleWidget*> loadedWidgets;
    
    UPROPERTY()
    TArray<USpeechBubbleWidget*> activeWidgets;

public:
    // Need this ticking to handle animation and state and position
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    virtual void NativeConstruct() override;

    void BeginMessage(FDialogueData messageData);
    void Advance();
    void Close();

};
