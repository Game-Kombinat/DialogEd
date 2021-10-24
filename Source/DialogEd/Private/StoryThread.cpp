// Copyright (c) Game Kombinat


#include "StoryThread.h"

UStoryThread::UStoryThread() {
    threadName = "Unnamed Thread";
    threadPointer = 0;
}

UStoryThread::UStoryThread(FString displayName) {
    threadName = displayName;
    threadPointer = 0;
}

void UStoryThread::AddCommand(const FParsedCommand command) {
    commandStack.Add(command);
}

bool UStoryThread::CanContinue() const {
    return threadPointer < commandStack.Num();
}

FParsedCommand UStoryThread::GetNext() {
    return commandStack[threadPointer++];
}

void UStoryThread::ResetThread() {
    threadPointer = 0;
}
