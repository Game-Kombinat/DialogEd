﻿// Copyright (c) Game Kombinat


#include "DialogueCommandRegister.h"

#include "Logging.h"

UClass* UDialogueCommandRegister::FindClassForName(const FString& name) const {
    const FString contextString;
    const auto row = commandMap->FindRow<FCommandRelation>(FName(name), *contextString);
    if (row) {
        return row->commandClass;
    }
    return nullptr;
}

UDialogueCommand* UDialogueCommandRegister::GetCommand(const FString name) {
    if (instantiatedCommands.Contains(name)) {
        return instantiatedCommands[name];
    }
    const UClass* classType = FindClassForName(name);
    if (!classType) {
        LOG_ERROR("unmapped command name discovered: %s", *name);
        return nullptr;
    }
    const auto newInstance = NewObject<UDialogueCommand>(this, classType, FName(name + "_instance"), RF_Transient);
    instantiatedCommands.Add(name, newInstance);
    return newInstance;
}

void UDialogueCommandRegister::OnBeginPlay() {
    // this is relevant mostly in editor. cleans up left over entries from last PIE session.
    instantiatedCommands.Empty();
}
