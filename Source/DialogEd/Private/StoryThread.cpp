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
}

bool UStoryThread::CanContinue() const {
    return threadPointer < commandStack.Num();
}

bool UStoryThread::IsRunning() const {
    return currentCommand.HasValidSetup() && !currentCommand.IsFinished();
}

FParsedCommand UStoryThread::GetNext() {
    return commandStack[threadPointer++];
}

void UStoryThread::ResetStoryThread() {
    threadPointer = 0;
    isPrimed = false;
}

FString UStoryThread::GetStoryThreadName() const {
    return threadName;
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

void UStoryThread::CleanupCommand() const {
    currentCommand.Cleanup();
}
