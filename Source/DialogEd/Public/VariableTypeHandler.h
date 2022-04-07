// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "VariableTypeHandler.generated.h"

///
/// Handles variables for the dialog process.
/// This can be overridden and custom implemented for
/// any number of custom variable types.
/// In script they are declared like so: typeName:variableName
/// We will use typename to identify which handler should be used
/// for this particular type of variable.
///
UCLASS(Abstract)
class DIALOGED_API UVariableTypeHandler : public UObject {
    GENERATED_BODY()
public:
    virtual int GetValue(FString variableName) PURE_VIRTUAL(return, return 0;);

    virtual void SetValue(FString variableName, int value) PURE_VIRTUAL(return);

    virtual void Init(class UStoryRunner* inRunner) PURE_VIRTUAL(return);

};
