// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "ParsedCommand.h"
#include "PreparedCommand.h"
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
    class UStoryAsset* storyAsset;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<FParsedCommand> commandStack;

    // index at which we're currently at in the stack
    UPROPERTY(VisibleAnywhere)
    int threadPointer;

    FPreparedCommand currentCommand;
    FString nextBranchId;

    bool isPrimed;
    
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
    TArray<class UDialogueActor*> actorsInThread;
    
    UStoryThread();
    explicit UStoryThread(FString displayName);

    void AddCommand(FParsedCommand command);

    void SetStoryAsset(UStoryAsset* story) { storyAsset = story; }

    bool CanContinue() const;

    bool IsRunning() const;

    bool IsBranching() const;

    FParsedCommand GetNext();
    FParsedCommand GetCurrent();
    
    void ResetStoryThread();
    FString GetStoryThreadName() const;

    void SetThreadName(const FString& newThreadName) {
        threadName = newThreadName;
    }

    /** Called from thread runner when the requested branching is being processed.*/
    void BranchingConsumed();

    bool IsPrimed() const;

    void Prime();

    /** Runs new command */
    bool RunCommand(FPreparedCommand command);

    /** Cleans up current command */
    void CleanupCommand();
    void AddActorInThread(class UDialogueActor* actor);
    void ClearActorsInThread();
    void SetBranchingTarget(int choice);
    FString GetBranchingTarget() const;
    UFUNCTION(BlueprintCallable)
    UStoryAsset* GetStoryAsset() const;
};
