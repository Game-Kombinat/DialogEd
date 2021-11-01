// Copyright (c) Game Kombinat


#include "MessageManager.h"

#include "Logging.h"


FMessageManager::FMessageManager(): messaging(nullptr) {
}

FMessageManager::~FMessageManager() {
}

void FMessageManager::Begin(FRuntimeDialogueData data) const {
    if (messaging) {
        messaging->BeginMessage(data);
    }
}

void FMessageManager::Begin(FRuntimeDialogueData data, FChoiceCallback receiveChoice) const {
    if (messaging) {
        messaging->BeginChoice(data, receiveChoice);
    }
}

void FMessageManager::Attach(FString message) {
    // not implemented just yet.
}

void FMessageManager::Advance() const {
    if (!messaging) {
        return;
    }
    messaging->Advance();
}

bool FMessageManager::IsDone() const {
    if (!messaging) {
        LOG_WARNING("messaging is null in MessageManager");
        return true;
    }
    return !messaging->IsDisplayingMessage();
}

void FMessageManager::Close() const {
    if (messaging) {
        messaging->Close();
    }
    
}
