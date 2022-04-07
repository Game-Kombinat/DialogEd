// Fill out your copyright notice in the Description page of Project Settings.


#include "DataContextVariableHandler.h"

void UDataContextVariableHandler::SetDataContext(UGameDataContext* ctx) {
    dataContext = ctx;
}

UGameDataContext* UDataContextVariableHandler::GetDataContext() {
    return dataContext;
}

int UDataContextVariableHandler::GetValue(FString variableName) {
    return dataContext->GetValue(variableName);
}

void UDataContextVariableHandler::SetValue(FString variableName, int value) {
    dataContext->ForceSetValue(variableName, value);
}
