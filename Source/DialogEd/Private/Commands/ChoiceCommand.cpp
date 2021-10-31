// Copyright (c) Game Kombinat


#include "Commands/ChoiceCommand.h"

UChoiceCommand::UChoiceCommand() {
    choiceReceived = false;
    playerChoice = -1;
}

void UChoiceCommand::Execute(UDialogueActor* target, TArray<FString> strings) {
    Super::Execute(target, strings);
}

void UChoiceCommand::Execute(UDialogueActor* target, FString arg) {
    Super::Execute(target, arg);
}

bool UChoiceCommand::IsFinished() {
    // return true when a choice from the player was received.
    return choiceReceived;
}

void UChoiceCommand::Cleanup() {
    choiceReceived = false;
    playerChoice = -1;
}

void UChoiceCommand::ReceiveChoice(const int choice) {
    choiceReceived = true;
    playerChoice = choice;
    // find a way to identify this branch
    // so when setting the choice to story thread,
    // we actually know where to set the thread pointer to or which subthread to run next.
    // myThread->
}
