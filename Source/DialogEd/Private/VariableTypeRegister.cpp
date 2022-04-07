// Fill out your copyright notice in the Description page of Project Settings.


#include "VariableTypeRegister.h"

#include "VariableTypeHandler.h"
#include "VariableTypeLookupRow.h"

void UVariableTypeRegister::Init(UWorld* inWorld, UDataTable* handlerMap) {
    world = inWorld;
    typeToHandlerMap = handlerMap;
}

UVariableTypeHandler* UVariableTypeRegister::GetHandler(const FString& type) {
    if (resolvedHandlers.Contains(type)) {
        return resolvedHandlers[type];
    }
    const UClass* cls = GetTypeClass(type);
    const auto instance = NewObject<UVariableTypeHandler>(this, cls);

    resolvedHandlers.Add(type, instance);

    return instance;
}

UClass* UVariableTypeRegister::GetTypeClass(const FString& type) const {
    if (!typeToHandlerMap) {
        return defaultTypeHandler;
    }
    const FString contextString;
    const auto row = typeToHandlerMap->FindRow<FVariableTypeLookupRow>(FName(type), contextString);
    if (!row) {
        return defaultTypeHandler;
    }
    return row->type;
}
