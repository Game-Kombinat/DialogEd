// Copyright (c) Game Kombinat


#include "PreparedCommand.h"

#include "DialogueActor.h"

FPreparedCommand::FPreparedCommand() {
    targetActor = nullptr;
    logic = nullptr;
}

FPreparedCommand::FPreparedCommand(UDialogueCommand* command, FString args, UDialogueActor* targetActor) {
    this->arguments = args;
    this->targetActor = targetActor;
    this->logic = command;
    arguments.ParseIntoArray(argumentList, *FString(" "));
}

FPreparedCommand::~FPreparedCommand() {
}

bool FPreparedCommand::HasValidSetup() const {
    return logic != nullptr && targetActor != nullptr;
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
