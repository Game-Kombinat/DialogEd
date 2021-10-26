// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "ParsedCommand.h"
#include "StoryThread.generated.h"

/**
 * A thread is a name and a stack of commands.
 * If a choice or other branching command is discovered, a thread will automatically end
 * with this node and divert to another thread, referenced by the branching command.
 *
 * This is part of raw parsed data from the input text. It is part of a story which is a piece of DataAsset.
 */
UCLASS(BlueprintType)
class DIALOGED_API UStoryThread : public UObject {
    GENERATED_BODY()
protected:
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString threadName;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<FParsedCommand> commandStack;

    // index at which we're currently at in the stack
    UPROPERTY(VisibleAnywhere)
    int threadPointer;

    bool isPrimed;
    
public:
    UStoryThread();
    explicit UStoryThread(FString displayName);

    void AddCommand(FParsedCommand command);

    bool CanContinue() const;

    FParsedCommand GetNext();
    
    void ResetStoryThread();
    FString GetStoryThreadName() const;

    void SetThreadName(const FString& newThreadName) {
        threadName = newThreadName;
    }

    bool IsPrimed() const;

    void Prime();


};
