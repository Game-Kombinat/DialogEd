// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StoryThread.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGED_API UStoryThread : public UObject {
    GENERATED_BODY()
    UPROPERTY(VisibleAnywhere)
    FString threadName;
    
    UPROPERTY(VisibleAnywhere)
    TArray<UClass*> commandStack;

    int threadPointer;
    
public:
    UStoryThread();
    explicit UStoryThread(FString displayName);

    void AddCommand(UClass* commandClass);

    bool CanContinue();

    class FPreparedCommand GetNext();
    

};
