// Copyright (c) Game Kombinat


#include "StoryThread.h"

FStoryThread::FStoryThread() {
    threadName = "Unnamed Thread";
    threadPointer = 0;
}

FStoryThread::FStoryThread(FString displayName) {
    threadName = displayName;
    threadPointer = 0;
}

void FStoryThread::AddCommand(const FParsedCommand command) {
    commandStack.Add(command);
}

bool FStoryThread::CanContinue() const {
    return threadPointer < commandStack.Num();
}

FParsedCommand FStoryThread::GetNext() {
    return commandStack[threadPointer++];
}

void FStoryThread::ResetThread() {
    threadPointer = 0;
}
