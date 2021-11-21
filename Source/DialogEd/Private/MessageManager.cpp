﻿// Copyright (c) Game Kombinat


#include "MessageManager.h"

#include "Logging.h"
#include "StoryThread.h"
#include "Ui/MessagingWidget.h"


UMessageManager::UMessageManager(): messaging(nullptr) {
}

UMessageManager::~UMessageManager() {
}

void UMessageManager::SetMessagingWidget(UMessagingWidget* widget) {
    LOG_INFO("Setting fresh messaging widget: %s", *widget->GetName())
    messaging = widget;
}

void UMessageManager::Begin(FRuntimeDialogueData data) const {
    if (messaging) {
        messaging->BeginMessage(data);
    }
}


void UMessageManager::Attach(FString message) {
    // not implemented just yet.
}

void UMessageManager::Advance() const {
    if (!messaging) {
        return;
    }
    messaging->Advance();
}

bool UMessageManager::IsDone() const {
    if (!messaging) {
        LOG_WARNING("messaging is null in MessageManager");
        return true;
    }
    return !messaging->IsDisplayingMessage();
}

void UMessageManager::Close() const {
    if (messaging) {
        messaging->Close();
    }
    
}

void UMessageManager::SetStoryThread(UStoryThread* story) {
    currentStoryThread = story;
    if (messaging) {
        messaging->SetCurrentStory(story);
    }
}

void UMessageManager::SetRelevantActors(const TArray<UDialogueActor*> actors) const {
    messaging->SetRelevantActors(actors);
}


void UMessageManager::RemoveFromViewport() const {
    messaging->RemoveFromViewport();
}

void UMessageManager::AddToViewport() const {
    messaging->AddToViewport(0);
    messaging->ForceLayoutPrepass();
}

void UMessageManager::SetDataContext(UGameDataContext* dc) {
    dataContext = dc;
}

UGameDataContext* UMessageManager::GetDataContext() {
    return dataContext;
}
