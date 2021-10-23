// Copyright (c) Game Kombinat


#include "Ui/MessagingWidget.h"

void UMessagingWidget::NativeTick(const FGeometry& myGeometry, float inDeltaTime) {
    Super::NativeTick(myGeometry, inDeltaTime);
}

void UMessagingWidget::NativeConstruct() {
    Super::NativeConstruct();
    for (int i = 0; i < 3; ++i) {
        auto bubbleWidget = Cast<USpeechBubbleWidget>(CreateWidgetInstance(*GetOwningPlayer(), bubbleType, FName("Bubble " + i)));
        loadedWidgets.Add(bubbleWidget);
    }
}

void UMessagingWidget::BeginMessage(FDialogueData messageData) {
    // get widgets from loadedWidgets list add to screen.
    // remove from loadedWidgets list.
    // add to activeWidgets list and start the widget rendering routine.
}

void UMessagingWidget::Advance() {
    // take the first widget from activeWidgets list and advance it
}

void UMessagingWidget::Close() {
    // take the first widget from activeWidgets list and close it.
    // remove the widget from screen and from activeWidgets list.
    // add to loadedWidgets list.
}
