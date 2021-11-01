// Copyright (c) Game Kombinat


#include "Commands/ChoiceCommand.h"

#include "Logging.h"

UChoiceCommand::UChoiceCommand() {
    choiceReceived = false;
    playerChoice = -1;
}

void UChoiceCommand::Execute(UDialogueActor* target, TArray<FString> strings) {
    LOG_WARNING("Called Choice command with argument array. Using first element!");
    
    Execute(target, strings[0]);
}

void UChoiceCommand::Execute(UDialogueActor* target, FString arg) {
    choiceReceived = false;
    playerChoice = -1;
    
    FRuntimeDialogueData d = FRuntimeDialogueData(target, arg);
    d.SetBranches(branches);
    choiceCallback.Unbind();
    choiceCallback.BindDynamic(this, &UChoiceCommand::ReceiveChoice);
    //choiceCallback.BindUObject();
    messageManager->Begin(d, choiceCallback);
}

bool UChoiceCommand::IsFinished() {
    // return true when a choice from the player was received.
    return messageManager->IsDone();
}

void UChoiceCommand::Cleanup() {
    
}

void UChoiceCommand::ReceiveChoice(int choice) {
    choiceReceived = true;
    playerChoice = choice;
    myThread->SetBranchingTarget(choice);
    messageManager->Close();
}
