// Fill out your copyright notice in the Description page of Project Settings.


#include "DataContextVariableHandler.h"

#include "StoryRunner.h"


UGameDataContext* UDataContextVariableHandler::GetDataContext() {
    return runner->GetDataContext();
}

int UDataContextVariableHandler::GetValue(FString variableName) {
    return runner->GetDataContext()->GetValue(variableName);
}

void UDataContextVariableHandler::SetValue(FString variableName, int value) {
    runner->GetDataContext()->ForceSetValue(variableName, value);
}

void UDataContextVariableHandler::Init(UStoryRunner* inRunner) {
    runner = inRunner;
}
