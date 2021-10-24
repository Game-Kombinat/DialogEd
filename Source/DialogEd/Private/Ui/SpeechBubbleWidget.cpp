// Copyright (c) Game Kombinat


#include "Ui/SpeechBubbleWidget.h"

#include "Anim8Action.h"
#include "Logging.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/RichTextBlock.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"

void USpeechBubbleWidget::MakeVisible() {
    currentState = EBubbleState::Opening;
    FAnim8Sample sample;
    sample.BindLambda([&](const float t) {
        verticalBox->SetRenderOpacity(t);
        verticalBox->SetRenderScale(FMath::Lerp(FVector2D::ZeroVector, FVector2D::UnitVector, t));
    });

    FAnim8Done done;
    done.BindLambda([&]() {
        // it makes no sense atm, it should be "Open" but we _will_ have a typewriter effect
        // at some point and then this is where the writing effect will begin.
        currentState = EBubbleState::Writing;
    });
    LOG_INFO("Begin showing bubble!")
    FInterpolator::Anim8(GetWorld(), .15, true, sample, done);
}

void USpeechBubbleWidget::MakeInvisible() {
    currentState = EBubbleState::Closing;

    FInterpolator::Anim8(GetWorld(), .15, true,
                         [this](const float t) {
                             verticalBox->SetRenderOpacity(t);
                             verticalBox->SetRenderScale(FMath::Lerp(FVector2D::UnitVector, FVector2D::ZeroVector, t));

                         },
                         [this]() {
                             currentState = EBubbleState::Closed;
                             RemoveFromViewport();
                         });
}

void USpeechBubbleWidget::PrepareBubble() {
    // Because we're currently not in the know how to properly do
    // a  typewriting effect that doesn't suck total ass,
    // it's gotta have to do without for now.
    // Otherwise this would be the place to set it up.
    verticalBox->SetRenderOpacity(0);
    verticalBox->SetRenderScale(FVector2D::ZeroVector);
    currentState = EBubbleState::Closed;
}

void USpeechBubbleWidget::NativeConstruct() {
    Super::NativeConstruct();
    interp = FInterpolator(0.15f, GetWorld());
}

FVector2D USpeechBubbleWidget::GetUnscaledSize() const {
    return verticalBox->GetDesiredSize();
    //return bubbleContainer->GetDesiredSize();
}

void USpeechBubbleWidget::SetSizeBoxBounds(FVector2D maxDesired) const {
    bubbleContainer->SetMaxDesiredWidth(maxDesired.X);
    bubbleContainer->SetMaxDesiredHeight(maxDesired.Y);
}

void USpeechBubbleWidget::SetPosition(FVector2D position) {
    const auto canvasSlot = Cast<UCanvasPanelSlot>(verticalBox->Slot);
    if (!canvasSlot) {
        LOG_ERROR("Slot for vertical box is not a canvas panel slot ... cannot set speech bubble position.");
        return;
    }
    // canvasSlot->SetPosition(position);
    canvasSlot->SetDesiredPosition(position);
}

void USpeechBubbleWidget::PrepareForDisplay(FDialogueData dataToShow, FVector2D maxBoundSize, int drawOrder) {
    AddToViewport(drawOrder);
    currentDialogueData = dataToShow;
    message->TakeWidget(); // ensures underlying slate widget exists ...
    message->SetText(FText::FromString(dataToShow.message));
    
    auto defaultStyle = message->GetCurrentDefaultTextStyle();
    message->SetDefaultColorAndOpacity(FSlateColor(dataToShow.textBaseColor));
    //message->SetDefaultTextStyle(defaultStyle);

    verticalBox->SetRenderOpacity(1);
    verticalBox->SetRenderScale(FVector2D::UnitVector);
    SetSizeBoxBounds(maxBoundSize);
    message->ForceLayoutPrepass();
}

void USpeechBubbleWidget::Show() {
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
