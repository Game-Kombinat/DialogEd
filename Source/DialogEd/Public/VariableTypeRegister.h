// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
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

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UVariableTypeHandler> defaultTypeHandler;
    
    UPROPERTY()
    UVariableTypeHandler* defaultTypeHandlerInstance;

    TMap<FString, class UVariableTypeHandler*> resolvedHandlers;
    UPROPERTY(VisibleAnywhere)
    UWorld* world;
    
public:
    void Init(UWorld* inWorld, UDataTable* handlerMap);

    UVariableTypeHandler* GetHandler(const FString& type);

private:
    UClass* GetTypeClass(const FString& type) const;

};
