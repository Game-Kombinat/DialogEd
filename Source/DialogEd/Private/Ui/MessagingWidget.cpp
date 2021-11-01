// Copyright (c) Game Kombinat


#include "Ui/MessagingWidget.h"

#include "EventManager.h"
#include "Logging.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UMessagingWidget::NativeTick(const FGeometry& myGeometry, float inDeltaTime) {
    Super::NativeTick(myGeometry, inDeltaTime);
    geometry = myGeometry;
    const FVector2D canvasSize = geometry.GetLocalSize();
    // at this time, place all active widgets in the bottom of the screen, centered.
    // this is done in BeginMessage but in a future time, where we get actual speech bubbles,
    // we gotta update positions and we gotta do it here.
    for (auto widget : activeWidgets) {
        const FVector2D widgetSize = widget->GetUnscaledSize();
        float horizontalMiddle = canvasSize.X / 2.0f;
        float widgetHalfLength = widgetSize.X / 2.0f;
        
        float widgetX = horizontalMiddle - widgetHalfLength;
        float widgetY = canvasSize.Y - (widgetSize.Y + (widgetSize.Y * .1f) );

        widget->SetRenderTranslation(FVector2D(widgetX, widgetY));
    }
    
}

void UMessagingWidget::NativeConstruct() {
    Super::NativeConstruct();
    for (int i = 0; i < 3; ++i) {
        auto bubbleWidget = Cast<USpeechBubbleWidget>(CreateWidgetInstance(*this, bubbleType, FName(FString::Format(TEXT("Bubble {0}"), {i}))));
        loadedWidgets.Add(bubbleWidget);
    }
    LOG_INFO("Messaging widget constructed: %s", *GetName());
}

void UMessagingWidget::NativeDestruct() {
    Super::NativeDestruct();
    // Not sure we need it. Unreal usually cleans up all the stuff on its own but lets go with it.
    loadedWidgets.Empty();
    activeWidgets.Empty();
}

void UMessagingWidget::BeginMessage(FRuntimeDialogueData messageData) {
    // get widgets from loadedWidgets list add to screen.
    // remove from loadedWidgets list.
    // add to activeWidgets list and start the widget rendering routine.
    if (loadedWidgets.Num() == 0) {
        LOG_WARNING("No more widgets available to show message");
        return;
    }
    const auto widget = loadedWidgets[0];
    const FVector2D canvasSize = GetCachedGeometry().GetLocalSize();
    // LOG_INFO("Canvas Size x: %f, y: %f", canvasSize.X, canvasSize.Y);
    // LOG_INFO("Setting max bubble size to x: %f, y: %f", canvasSize.X / 2.25f, canvasSize.Y / 2.25f);
    
    loadedWidgets.Remove(widget);
    activeWidgets.Add(widget);
    widget->ClearChoiceSelectionCallback();
    widget->PrepareForDisplay(messageData, FVector2D(canvasSize.X / 2.25f, canvasSize.Y / 2.25f), activeWidgets.Num()); // adds itself to viewport in the beginning.
    widget->ForceLayoutPrepass();
    ForceLayoutPrepass();
    widget->Show();
}

void UMessagingWidget::BeginChoice(const FRuntimeDialogueData data, FChoiceCallback receiveChoice) {
    // get widgets from loadedWidgets list add to screen.
    // remove from loadedWidgets list.
    // add to activeWidgets list and start the widget rendering routine.
    if (loadedWidgets.Num() == 0) {
        LOG_WARNING("No more widgets available to show message");
        return;
    }
    const auto widget = loadedWidgets[0];
    const FVector2D canvasSize = GetCachedGeometry().GetLocalSize();
    // LOG_INFO("Canvas Size x: %f, y: %f", canvasSize.X, canvasSize.Y);
    // LOG_INFO("Setting max bubble size to x: %f, y: %f", canvasSize.X / 2.25f, canvasSize.Y / 2.25f);
    
    loadedWidgets.Remove(widget);
    activeWidgets.Add(widget);
    widget->SetChoiceSelectionCallback(receiveChoice);
    widget->PrepareForDisplay(data, FVector2D(canvasSize.X / 2.25f, canvasSize.Y / 2.25f), activeWidgets.Num()); // adds itself to viewport in the beginning.
    widget->ForceLayoutPrepass();
    ForceLayoutPrepass();
    widget->Show();
}

void UMessagingWidget::Advance() {
    // take the first widget from activeWidgets list and advance it
    if (activeWidgets.Num() == 0) {
        LOG_WARNING("No active widgets available for advancing.");
        return;
    }
    if (activeWidgets[0]->IsWriting()) {
        activeWidgets[0]->Advance();
    }
    else {
        Close();
    }
}

void UMessagingWidget::Close() {
    // take the first widget from activeWidgets list and close it.
    // remove the widget from screen and from activeWidgets list.
    // add to loadedWidgets list.
    if (activeWidgets.Num() == 0) {
        LOG_WARNING("No widgets left to close.");
        return;
    }
    const auto widget = activeWidgets[0];
    widget->Hide(); // removes itself from viewport at the end
    activeWidgets.Remove(widget);
    loadedWidgets.Add(widget);
}

void UMessagingWidget::BeginListenToInputAction(FName actionName, FOnInputAction callback) {
    ListenForInputAction(actionName, EInputEvent::IE_Pressed, true, callback);
}

void UMessagingWidget::StopListeningToInputAction(FName actionName) {
    StopListeningForInputAction(actionName, EInputEvent::IE_Pressed);
}

bool UMessagingWidget::IsDisplayingMessage() const {
    if (activeWidgets.Num() == 0 && loadedWidgets.Num() == 0) {
        return false;
    }
    
    for (auto widget : activeWidgets) {
        if (widget->IsInViewport()) {
            return true;
        }
    }

    // There is a short period (animation) where widgets are not in active list
    // but are still on the viewport.
    for (auto widget : loadedWidgets) {
        if (widget->IsInViewport()) {
            return true;
        }
    }
    LOG_INFO("No widgets in viewport, calling this done.")
    return false;
}
