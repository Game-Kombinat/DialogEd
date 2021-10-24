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

    UPROPERTY(VisibleAnywhere)
    TArray<USpeechBubbleWidget*> loadedWidgets;
    
    UPROPERTY(VisibleAnywhere)
    TArray<USpeechBubbleWidget*> activeWidgets;
    
    FGeometry geometry;

public:
    // Need this ticking to handle animation and state and position
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable)
    void BeginMessage(FDialogueData messageData);
    UFUNCTION(BlueprintCallable)
    void Advance();
    UFUNCTION(BlueprintCallable)
    void Close();

};
