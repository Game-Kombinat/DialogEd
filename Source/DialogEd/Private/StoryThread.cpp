// Copyright (c) Game Kombinat


#include "StoryThread.h"

#include "PreparedCommand.h"

UStoryThread::UStoryThread() {
    threadName = "Unnamed Thread";
    threadPointer = 0;
}

UStoryThread::UStoryThread(FString displayName) {
    threadName = displayName;
    threadPointer = 0;
}

void UStoryThread::AddCommand(UClass* commandClass) {
    commandStack.Add(commandClass);
}

bool UStoryThread::CanContinue() {
    return threadPointer < commandStack.Num();
}

FPreparedCommand UStoryThread::GetNext() {
    // auto c = NewObject<IDialogueCommand>(this, commandStack[threadPointer++]);
    // auto prep = FPreparedCommand(c);
    // return commandStack[threadPointer++];
    // todo: this is shit, shouldn't split here.
    return FPreparedCommand(nullptr, TArray<FString>(), nullptr);
}
