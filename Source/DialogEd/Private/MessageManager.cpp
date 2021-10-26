// Copyright (c) Game Kombinat


#include "MessageManager.h"

#include "Logging.h"


FMessageManager::FMessageManager(): hasAdvanced(false), messaging(nullptr) {
}

FMessageManager::~FMessageManager() {
}

void FMessageManager::Begin(FRuntimeDialogueData data) {
    if (messaging) {
        hasAdvanced = false;
        messaging->BeginMessage(data);
    }
}

void FMessageManager::Attach(FString message) {
    // not implemented just yet.
}

void FMessageManager::Advance() {
    if (!messaging) {
        return;
    }
    if (!hasAdvanced) {
        messaging->Advance();
        hasAdvanced = true;
    }
    else {
        messaging->Close();
    }
    
}

bool FMessageManager::IsDone() const {
    if (!messaging) {
        LOG_WARNING("messaging is null in MessageManager");
        return true;
    }
    return !messaging->IsDisplayingMessage();
}
