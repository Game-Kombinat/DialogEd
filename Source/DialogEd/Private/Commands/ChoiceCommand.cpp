// Copyright (c) Game Kombinat


#include "Commands/ChoiceCommand.h"

#include "Logging.h"
#include "MessageManager.h"
#include "StoryRunner.h"
#include "Commands/RuntimeDialogueData.h"

UChoiceCommand::UChoiceCommand() {
    choiceReceived = false;
    playerChoice = -1;
}

void UChoiceCommand::Execute(TSharedPtr<FParsedCommand> cmd) {
    choiceReceived = false;
    playerChoice = -1;
    
    FRuntimeDialogueData d = FRuntimeDialogueData(storyRunner->GetDialogueActor(cmd->targetActor), cmd->trimmedArgumentList);
    
    d.SetBranches(cmd->branches);
    //choiceCallback.BindUObject();
    storyRunner->GetMessageManager()->Begin(d);
}

bool UChoiceCommand::IsFinished() {
    // return true when a choice from the player was received.
    return storyRunner->GetMessageManager()->IsDone();
}

void UChoiceCommand::Cleanup() {
    
}

void UChoiceCommand::ReceiveChoice(int choice) {
    choiceReceived = true;
    playerChoice = choice;
    myThread->SetBranchingTarget(choice);
    storyRunner->GetMessageManager()->Close();
}
