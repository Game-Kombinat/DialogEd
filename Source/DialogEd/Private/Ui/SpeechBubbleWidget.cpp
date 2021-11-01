// Copyright (c) Game Kombinat


#include "Ui/SpeechBubbleWidget.h"

#include "Anim8Action.h"
#include "Logging.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/RichTextBlock.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/VerticalBox.h"
#include "Framework/Text/RichTextLayoutMarshaller.h"

void USpeechBubbleWidget::MakeVisible() {
    // see https://github.com/redxdev/UnrealRichTextDialogueBox for possible text typing solution ...
    
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

void USpeechBubbleWidget::PrepareChoices(TArray<FDialogueBranchId> branches) {
    choicesPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    const int numBranches = FMath::Min(4, branches.Num());
    // TArray<UText> buttonLabels;
    // buttonLabels[0].GetChildAt(0)
    for (int i = 0; i < 4; ++i) {
        if (i >= numBranches) {
            choiceButtons[i]->SetVisibility(ESlateVisibility::Collapsed);
        }
        else {
            choiceButtons[i]->SetVisibility(ESlateVisibility::Visible);
            choiceButtonLabels[i]->SetText(FText::FromString(branches[i].title));
        }
    }
}

void USpeechBubbleWidget::HideChoices() {
    choicesPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void USpeechBubbleWidget::NativeConstruct() {
    Super::NativeConstruct();
    interp = FInterpolator(0.15f, GetWorld());
    // create button references because umg widgets ain't blueprintable and cannot e referenced in TArray from inspector
    int numChildren = choicesPanel->GetChildrenCount();
    for (int i = 0; i < numChildren; ++i) {
        auto button = Cast<UButton>(choicesPanel->GetChildAt(i));
        if (!button) {
            LOG_ERROR("Found non button in choices panel child");
            continue;
        }

        choiceButtons.Add(button);
        auto label = Cast<UTextBlock>(button->GetChildAt(0));
        if (!label) {
            LOG_ERROR("Found non TextBlock in choices button first child");
            continue;
        }
        choiceButtonLabels.Add(label);

        TScriptDelegate<> d;
        switch (i) {
            default:
            case 0:
                d.BindUFunction(this, "OnFirstChoice");
                button->OnClicked.Remove(this, "OnFirstChoice");
                break;
            case 1:
                d.BindUFunction(this, "OnSecondChoice");
                button->OnClicked.Remove(this, "OnSecondChoice");
                break;
            case 2:
                d.BindUFunction(this, "OnThirdChoice");
                button->OnClicked.Remove(this, "OnThirdChoice");
                break;
            case 3:
                d.BindUFunction(this, "OnFourthChoice");
                button->OnClicked.Remove(this, "OnFourthChoice");
                break;
                
        }
        button->OnClicked.Add(d);
    }
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

void USpeechBubbleWidget::ClearChoiceSelectionCallback() {
    choiceSelectionCallback.Unbind();
}

void USpeechBubbleWidget::SetChoiceSelectionCallback(FChoiceCallback receiveChoice) {
    choiceSelectionCallback = receiveChoice;
}

void USpeechBubbleWidget::OnFirstChoice() const {
    if (choiceSelectionCallback.IsBound()) {
        choiceSelectionCallback.Execute(0);
    }
}

void USpeechBubbleWidget::OnSecondChoice() const {
    if (choiceSelectionCallback.IsBound()) {
        choiceSelectionCallback.Execute(1);
    }
}

void USpeechBubbleWidget::OnThirdChoice() const {
    if (choiceSelectionCallback.IsBound()) {
            choiceSelectionCallback.Execute(2);
    }
}

void USpeechBubbleWidget::OnFourthChoice() const {
    if (choiceSelectionCallback.IsBound()) {
            choiceSelectionCallback.Execute(3);
    }
}

void USpeechBubbleWidget::PrepareForDisplay(FRuntimeDialogueData dataToShow, FVector2D maxBoundSize, int drawOrder) {
    AddToViewport(drawOrder);
    currentDialogueData = dataToShow;
    message->TakeWidget(); // ensures underlying slate widget exists ...
    message->SetText(FText::FromString(dataToShow.message));
    if (dataToShow.branchData.Num() > 0) {
        PrepareChoices(dataToShow.branchData);
    }
    else {
        HideChoices();
    }

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
