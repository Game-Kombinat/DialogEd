// Copyright (c) Game Kombinat


#include "StoryThread.h"

UStoryThread::UStoryThread() {
    threadName = "Unnamed Thread";
    threadPointer = 0;
    isPrimed = false;
    currentCommand = FPreparedCommand();
}

UStoryThread::UStoryThread(FString displayName) {
    threadName = displayName;
    threadPointer = 0;
    isPrimed = false;
    currentCommand = FPreparedCommand();
}

void UStoryThread::AddCommand(const FParsedCommand command) {
    commandStack.Add(command);
    if (command.requiresActor && !actorsInThread.Contains(command.targetActor)) {
        actorsInThread.Add(command.targetActor);
    }
}

bool UStoryThread::CanContinue() const {
    return threadPointer < commandStack.Num() || IsBranching();
}

bool UStoryThread::IsRunning() const {
    return currentCommand.HasValidSetup() && !currentCommand.IsFinished();
}

bool UStoryThread::IsBranching() const {
    return !nextBranchId.IsEmpty();
}

FParsedCommand UStoryThread::GetNext() {
    return commandStack[threadPointer++];
}

FParsedCommand UStoryThread::GetCurrent() {
    // we increased threadPointer last, so to get current, we have to remove 1
    return commandStack[threadPointer - 1];
}

void UStoryThread::ResetStoryThread() {
    threadPointer = 0;
    isPrimed = false;
}

FString UStoryThread::GetStoryThreadName() const {
    return threadName;
}

void UStoryThread::BranchingConsumed() {
    nextBranchId = "";
}

bool UStoryThread::IsPrimed() const {
    return isPrimed;
}

void UStoryThread::Prime() {
    isPrimed = true;
}

bool UStoryThread::RunCommand(FPreparedCommand command) {
    if (command.Verify()) {
        currentCommand = command;
        currentCommand.Run();
        return true;
    }
    return false;
    
}

void UStoryThread::CleanupCommand() {
    currentCommand.Cleanup();
}

void UStoryThread::SetBranchingTarget(int choice) {
    FParsedCommand current = GetCurrent();
    auto selectedChoice = current.branches[choice];
    nextBranchId = selectedChoice.branchId;
    // put the branching info here so runner can query if branching is required.
    // then put another thread onto its story thread stack that is executed.
    // when done thread is removed from stack.
    // when stack is empty, runner will close.
}

FString UStoryThread::GetBranchingTarget() const {
    return nextBranchId;
}

UStoryAsset* UStoryThread::GetStoryAsset() const {
    return storyAsset;
}