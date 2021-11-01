// Copyright (c) Game Kombinat


#include "Commands/SpeakCommand.h"

#include "DialogueActor.h"
#include "Logging.h"
#include "Ui/MessagingWidget.h"


USpeakCommand::USpeakCommand() {
}

USpeakCommand::~USpeakCommand() {
}

void USpeakCommand::Execute(UDialogueActor* target, TArray<FString> args) {
    LOG_WARNING("Received message as string list!")
    if (args.Num() > 0) {
        Execute(target, args[0]);
    }
    else {
        LOG_WARNING("Argument list in Speak command is empty.")
    }
}

void USpeakCommand::Execute(UDialogueActor* target, FString arg) {
    if (!messageManager) {
        LOG_ERROR("No MessageManager in Speak command.")
        return;
    }
    inputActionDelegate.BindDynamic(this, &USpeakCommand::OnActionPressed);
    messageManager->messaging->BeginListenToInputAction(messageManager->GetActionName(), inputActionDelegate);
    
    messageManager->Begin(FRuntimeDialogueData(target, arg));
}

bool USpeakCommand::IsFinished() {
    if (!messageManager) {
        LOG_WARNING("No message manager in SpeakCommand::IsFinished!");
        return true;
    }
    return messageManager->IsDone();
}

void USpeakCommand::Cleanup() {
    messageManager->messaging->StopListeningToInputAction(messageManager->GetActionName());
    inputActionDelegate.Unbind();
}

void USpeakCommand::OnActionPressed() {
    messageManager->Advance();
}
