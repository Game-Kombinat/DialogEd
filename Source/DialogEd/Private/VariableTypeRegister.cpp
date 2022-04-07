// Fill out your copyright notice in the Description page of Project Settings.


#include "VariableTypeRegister.h"

#include "DataContextVariableHandler.h"
#include "Logging.h"
#include "StoryRunner.h"
#include "VariableTypeHandler.h"
#include "VariableTypeLookupRow.h"

UVariableTypeRegister::UVariableTypeRegister() {
    defaultTypeHandler = UDataContextVariableHandler::StaticClass();
}

void UVariableTypeRegister::Init(UStoryRunner* inRunner, UDataTable* handlerMap) {
    world = inRunner->GetWorld();
    runner = inRunner;
    typeToHandlerMap = handlerMap;
}

UVariableTypeHandler* UVariableTypeRegister::GetHandler(const FString& type) {
    LOG_INFO("Attemtping to get variable type handler for %s", *type);
    if (resolvedHandlers.Contains(type)) {
        return resolvedHandlers[type];
    }
    // note: this creates default handlers for every unknown / unmapped type.
    // ideally we don't have any of those. But still, it's a thing that could be optimised.
    const UClass* cls = GetTypeClass(type);
    const auto instance = NewObject<UVariableTypeHandler>(this, cls);
    instance->Init(runner);

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
