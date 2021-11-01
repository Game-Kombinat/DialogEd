// Copyright (c) Game Kombinat


#include "MessageManager.h"

#include "Logging.h"
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

void UMessageManager::Begin(FRuntimeDialogueData data, FChoiceCallback receiveChoice) const {
    if (messaging) {
        messaging->BeginChoice(data, receiveChoice);
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
