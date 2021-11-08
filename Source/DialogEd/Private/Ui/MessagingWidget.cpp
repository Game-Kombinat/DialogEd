// Copyright (c) Game Kombinat


#include "Ui/MessagingWidget.h"

void UMessagingWidget::SetCurrentStory(UStoryThread* storyThread) {
     currentStoryThread = storyThread;
}

void UMessagingWidget::SetRelevantActors(const TArray<UDialogueActor*> actors) {
     relevantActors = actors;
}
