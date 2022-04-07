// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataContextContainer.h"
#include "VariableTypeHandler.h"
#include "DataContextVariableHandler.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGED_API UDataContextVariableHandler : public UVariableTypeHandler, public IDataContextContainer{
    GENERATED_BODY()
protected:
    UPROPERTY(VisibleAnywhere)
    class UStoryRunner* runner;
    
public:
    virtual UGameDataContext* GetDataContext() override;
    virtual int GetValue(FString variableName) override;
    virtual void SetValue(FString variableName, int value) override;
    virtual void Init(class UStoryRunner* inRunner) override;
};
