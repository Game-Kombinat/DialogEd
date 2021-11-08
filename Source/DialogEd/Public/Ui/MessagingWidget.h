// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "MessageManager.h"
#include "Blueprint/UserWidget.h"
#include "MessagingWidget.generated.h"

/**
 * Handles the rendering of speech bubbles.
 */
UCLASS(Abstract)
class DIALOGED_API UMessagingWidget : public UUserWidget {
    GENERATED_BODY()
protected:
    UPROPERTY()
    class UStoryThread* currentStoryThread;

    UPROPERTY()
    TArray<class UDialogueActor*> relevantActors;

public:
    // Need this ticking to handle animation and state and position
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override PURE_VIRTUAL(UMessagingWidget::NativeTick);
    virtual void NativeConstruct() override PURE_VIRTUAL(UMessagingWidget::NativeTick);
    virtual void NativeDestruct() override PURE_VIRTUAL(UMessagingWidget::NativeTick);

    virtual void BeginMessage(FRuntimeDialogueData messageData) PURE_VIRTUAL(UMessagingWidget::NativeTick);
    
    virtual void Advance() PURE_VIRTUAL(UMessagingWidget::NativeTick);
    virtual void Close() PURE_VIRTUAL(UMessagingWidget::NativeTick);

    /**
     * Returns true for as long as a message is still being rendered.
     * That is: In any state but the closing / closed state.
     */
    virtual bool IsDisplayingMessage() const PURE_VIRTUAL(UMessagingWidget::NativeTick, return true;);

    /** Callback when a choice / braching was selected. */
    virtual void BranchSelected(int choice) PURE_VIRTUAL(UMessagingWidget::BranchSelected);

    /** Callback when we should proceed. (next message) */
    virtual void OnProceed() PURE_VIRTUAL(UMessagingWidget::OnProceed);

    void SetCurrentStory(UStoryThread* storyThread);

    void SetRelevantActors(TArray<UDialogueActor*> actors);

};
