// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "DialogueData.h"
#include "Interpolator.h"
#include "SpeechBubbleTypes.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "SpeechBubbleWidget.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGED_API USpeechBubbleWidget : public UUserWidget {
    GENERATED_BODY()
protected:
    // this is what is in unity known s canvasgroup. or at least it can do the same thing with the alpha value.
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category="Bubble Setup")
    class UBorder* bubbleFrame; 
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category="Bubble Setup")
    class URichTextBlock* message;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category="Bubble Setup")
    class UImage* tail;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Runtime References")
    class UDialogueActor* dialogueActor;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Runtime References")
    UUserWidget* parentWidget;

    FDialogueData currentDialogueData;

    FInterpolator interp;
    EBubbleState currentState;
    
private:
    bool isWriting, waitingForInput; // todo: these can be covered by the bubble state as far as I can tell.
    /**
     * Works in interpolator and fades scale and transparency of speech bubble.
     * Returns true as soon as interpolator is done. False otherwise (which means, call again next tick)
     */
    void MakeVisible();

    /**
     * Works in interpolator and fades scale and transparency of speech bubble.
     * Returns true as soon as interpolator is done. False otherwise (which means, call again next tick)
     */
    void MakeInvisible();

    /**
     * Puts the speech bubble in a state that can be made visible.
     * Sets the text and makes it ready for typing animation.
     */
    void PrepareBubble();

public:
    /**
     * Kicks off a process on this speech bubble to show the dialogue data.
     */
    void Show(FDialogueData dataToShow);
    void Hide();
    
    // Need this ticking to handle animation and state and position
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    virtual void NativeConstruct() override;
};
