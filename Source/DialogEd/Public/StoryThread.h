// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "ParsedCommand.h"
#include "UObject/Object.h"
#include "StoryThread.generated.h"

/**
 * A thread is a name and a stack of commands.
 * If a choice or other branching command is discovered, a thread will automatically end
 * with this node and divert to another thread, referenced by the branching command.
 *
 * This is part of raw parsed data from the input text. It is part of a story which is a piece of DataAsset.
 */
UCLASS()
class DIALOGED_API UStoryThread : public UObject {
protected:
    GENERATED_BODY()
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString threadName;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<FParsedCommand> commandStack;

    // index at which we're currently at in the stack
    UPROPERTY(VisibleAnywhere)
    int threadPointer;
    
public:
    UStoryThread();
    explicit UStoryThread(FString displayName);

    void AddCommand(FParsedCommand command);

    bool CanContinue() const;

    FParsedCommand GetNext();
    
    void ResetThread();


};
