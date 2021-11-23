// Copyright (c) Game Kombinat


#include "Commands/SpeakCommand.h"

#include "DialogueActor.h"
#include "Logging.h"
#include "StoryRunner.h"
#include "Ui/MessagingWidget.h"


USpeakCommand::USpeakCommand() {
}

USpeakCommand::~USpeakCommand() {
}


void USpeakCommand::Execute(TSharedRef<FParsedCommand> cmd) {
    if (!storyRunner) {
        LOG_ERROR("No MessageManager in Speak command.")
        return;
    }
    UDialogueActor* dactor = storyRunner->GetDialogueActor(cmd->targetActor);
    myThread->AddActorInThread(dactor);
    storyRunner->GetMessageManager()->Begin(FRuntimeDialogueData(dactor, cmd->trimmedArgumentList));
}

bool USpeakCommand::IsFinished() {
    if (!storyRunner) {
        LOG_WARNING("No message manager in SpeakCommand::IsFinished!");
        return true;
    }
    return storyRunner->GetMessageManager()->IsDone();
}

void USpeakCommand::Cleanup() {
}