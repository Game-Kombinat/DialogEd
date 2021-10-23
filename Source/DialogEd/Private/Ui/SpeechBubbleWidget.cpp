// Copyright (c) Game Kombinat


#include "Ui/SpeechBubbleWidget.h"

#include "Components/RichTextBlock.h"

void USpeechBubbleWidget::MakeVisible() {
    currentState = EBubbleState::Opening;
    
    FLinearColor startTintColor = FLinearColor(1, 1, 1, 0);
    FLinearColor endTintColor = FLinearColor(1, 1, 1, 1);
    FVector2D startScale = FVector2D::ZeroVector;
    FVector2D endScale = FVector2D::UnitVector;

    FInterpolator::Anim8(GetWorld(), .15, true,
                         [&, startTintColor, endTintColor, startScale, endScale](const float t) {
                             
                             bubbleFrame->Background.TintColor = FLinearColor::LerpUsingHSV(startTintColor, endTintColor, t);
                             bubbleFrame->RenderTransform.Scale = FMath::Lerp(startScale, endScale, t);
                         },
                         [&]() {
                             // it makes no sense atm, it should be "Open" but we _will_ have a typewriter effect
                             // at some point and then this is where the writing effect will begin.
                             currentState = EBubbleState::Writing;
                         });
}

void USpeechBubbleWidget::MakeInvisible() {
    currentState = EBubbleState::Closing;
    
    FLinearColor startTintColor = FLinearColor(1, 1, 1, 1);
    FLinearColor endTintColor = FLinearColor(1, 1, 1, 0);
    FVector2D startScale = FVector2D::UnitVector;
    FVector2D endScale = FVector2D::ZeroVector;

    FInterpolator::Anim8(GetWorld(), .15, true,
                         [&, startTintColor, endTintColor, startScale, endScale](const float t) {
                             bubbleFrame->Background.TintColor = FLinearColor::LerpUsingHSV(startTintColor, endTintColor, t);
                             bubbleFrame->RenderTransform.Scale = FMath::Lerp(startScale, endScale, t);
                         },
                         [&]() {
                             currentState = EBubbleState::Closed;
                         });
}

void USpeechBubbleWidget::PrepareBubble() {
    // Because we're currently not in the know how to properly do
    // a  typewriting effect that doesn't suck total ass,
    // it's gotta have to do without for now.
    // Otherwise this would be the place to set it up.
    bubbleFrame->Background.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0));
    bubbleFrame->RenderTransform.Scale = FVector2D::ZeroVector;
    currentState = EBubbleState::Closed;
}

void USpeechBubbleWidget::NativeConstruct() {
    Super::NativeConstruct();
    interp = FInterpolator(0.15f, GetWorld());
}

void USpeechBubbleWidget::Show(FDialogueData dataToShow) {
    currentDialogueData = dataToShow;
    message->SetText(FText::FromString(dataToShow.message));
    
    auto defaultStyle = message->GetDefaultTextStyle();
    defaultStyle.ColorAndOpacity = FSlateColor(dataToShow.textBaseColor);
    message->SetDefaultTextStyle(defaultStyle);
    PrepareBubble();
    MakeVisible();
}

void USpeechBubbleWidget::Hide() {
    MakeInvisible();
    currentDialogueData = FDialogueData();
}

void USpeechBubbleWidget::NativeTick(const FGeometry& myGeometry, float inDeltaTime) {
    Super::NativeTick(myGeometry, inDeltaTime);
    // todo: when we actually get speech bubbles, this is the place to update position and stuff
}
