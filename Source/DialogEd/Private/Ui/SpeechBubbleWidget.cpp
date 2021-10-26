// Copyright (c) Game Kombinat


#include "Ui/SpeechBubbleWidget.h"

#include "Anim8Action.h"
#include "GameMapsSettings.h"
#include "Logging.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/RichTextBlock.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"

void USpeechBubbleWidget::MakeVisible() {
    currentState = EBubbleState::Opening;
    // we have to use explicit delegates here because
    // of the lambdas capture reach not playing along when passing it down directly.
    FAnim8Sample sample;
    sample.BindLambda([&](const float t) {
        verticalBox->SetRenderOpacity(t);
        verticalBox->SetRenderScale(FMath::Lerp(FVector2D::ZeroVector, FVector2D::UnitVector, t));
    });

    FAnim8Done done;
    done.BindLambda([&]() {
        currentState = EBubbleState::Waiting; // it has to be "Writing" when we go text writer effect.
    });
    FInterpolator::Anim8(GetWorld(), .15, true, sample, done);
}

void USpeechBubbleWidget::MakeInvisible() {
    currentState = EBubbleState::Closing;
    // we have to use explicit delegates here because
    // of the lambdas capture reach not playing along when passing it down directly.
    FAnim8Sample sample;
    sample.BindLambda([&](const float t) {
        verticalBox->SetRenderOpacity(t);
        verticalBox->SetRenderScale(FMath::Lerp(FVector2D::UnitVector, FVector2D::ZeroVector, t));

    });

    FAnim8Done done;
    done.BindLambda([&]() {
        currentState = EBubbleState::Closed;
        RemoveFromViewport();
    });

    FInterpolator::Anim8(GetWorld(), .15, true,
        sample,
        done
    );
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

bool USpeechBubbleWidget::IsWriting() const {
    return currentState != EBubbleState::Opening && currentState == EBubbleState::Writing;
}

void USpeechBubbleWidget::Advance() {
    // todo: this would put writing to open state and make all writing text visible
    currentState = EBubbleState::Waiting;
}

void USpeechBubbleWidget::PrepareForDisplay(FRuntimeDialogueData dataToShow, FVector2D maxBoundSize, int drawOrder) {
    AddToViewport(drawOrder);
    currentDialogueData = dataToShow;
    message->TakeWidget(); // ensures underlying slate widget exists ...
    message->SetText(FText::FromString(dataToShow.message));

    auto defaultStyle = message->GetCurrentDefaultTextStyle();
    message->SetDefaultColorAndOpacity(FSlateColor(dataToShow.textBaseColor));

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
    currentDialogueData = FRuntimeDialogueData();
}

void USpeechBubbleWidget::NativeTick(const FGeometry& myGeometry, float inDeltaTime) {
    Super::NativeTick(myGeometry, inDeltaTime);
    // todo: when we actually get speech bubbles, this is the place to update position and stuff
}
