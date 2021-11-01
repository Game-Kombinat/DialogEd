// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "MessageManager.h"
#include "SpeechBubbleWidget.h"
#include "MessagingWidget.generated.h"

/**
 * Handles the rendering of speech bubbles.
 */
UCLASS()
class DIALOGED_API UMessagingWidget : public UUserWidget {
    GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Messaging Setup")
    TSubclassOf<USpeechBubbleWidget> bubbleType;

    UPROPERTY(VisibleAnywhere, Category="Messaging Setup")
    TArray<USpeechBubbleWidget*> loadedWidgets;
    
    UPROPERTY(VisibleAnywhere, Category="Messaging Setup")
    TArray<USpeechBubbleWidget*> activeWidgets;

    UPROPERTY(EditAnywhere, Category="Messaging Setup")
    FName proceedActionName;
    
    FGeometry geometry;

public:
    // Need this ticking to handle animation and state and position
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    void BeginMessage(FRuntimeDialogueData messageData);

    void BeginChoice(const FRuntimeDialogueData data, FChoiceCallback receiveChoice);
    
    UFUNCTION(BlueprintCallable)
    void Advance();
    UFUNCTION(BlueprintCallable)
    void Close();

    void BeginListenToInputAction(FName actionName, FOnInputAction callback);

    void StopListeningToInputAction(FName actionName);

    /**
     * Returns true for as long as a message is still being rendered.
     * That is: In any state but the closing / closed state.
     */
    UFUNCTION(BlueprintCallable)
    bool IsDisplayingMessage() const;

};
