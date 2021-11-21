// Copyright (c) Game Kombinat


#include "PreparedCommand.h"

#include "DialogueActor.h"
#include "DialogueCommand.h"

FPreparedCommand::FPreparedCommand() {
    arguments = "";
    targetActor = nullptr;
    logic = nullptr;
}

FPreparedCommand::FPreparedCommand(UDialogueCommand* cmd, FParsedCommand rawCmd, UDialogueActor* inTargetActor) {
    command = rawCmd;
    arguments = rawCmd.argumentList;
    targetActor = inTargetActor;
    logic = cmd;
    arguments.ParseIntoArray(argumentList, TEXT(" "));
}

FPreparedCommand::~FPreparedCommand() {
}

bool FPreparedCommand::HasValidSetup() const {
    return logic != nullptr && (targetActor != nullptr || !command.requiresActor);
}

bool FPreparedCommand::Verify() const {
    if (!HasValidSetup()) {
        return false;
    }
    // check target actor type being correct.
    const auto requiredClass = logic->TargetActorType();
    if (requiredClass && !targetActor->GetOwner()->IsA(requiredClass)) {
        return false;
    }

    // check arguments
    return (argumentList.Num() >= logic->MinArguments() || logic->MinArguments() < 0) && (argumentList.Num() <= logic->MaxArguments() || logic->MaxArguments() < 0);
}

bool FPreparedCommand::IsFinished() const {
    if (!logic) {
        return true;
    }
    return logic->IsFinished();
}

void FPreparedCommand::Run() const {
    if (logic->WantsArgumentsAsList()) {
        logic->Execute(targetActor, argumentList);
    }
    else {
        logic->Execute(targetActor, arguments);
    }

}

void FPreparedCommand::Cleanup() {
    if (logic) {
        logic->Cleanup();
    }
    logic = nullptr;
    targetActor = nullptr;
    
}
