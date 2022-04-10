// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "VariableTypeRegister.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class DIALOGED_API UVariableTypeRegister : public UObject {
    GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UDataTable* typeToHandlerMap;

    UPROPERTY(EditAnywhere, Transient)
    class UStoryRunner* runner;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UVariableTypeHandler> defaultTypeHandler;

    UPROPERTY()
    TMap<FString, class UVariableTypeHandler*> resolvedHandlers;
    
    UPROPERTY(VisibleAnywhere)
    UWorld* world;
    
public:
    UVariableTypeRegister();
    void Init(class UStoryRunner* inRunner, UDataTable* handlerMap);

    UVariableTypeHandler* GetHandler(const FString& type);

private:
    UClass* GetTypeClass(const FString& type) const;

};
