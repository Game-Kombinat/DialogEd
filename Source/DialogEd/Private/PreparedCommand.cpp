// Copyright (c) Game Kombinat


#include "PreparedCommand.h"

FPreparedCommand::FPreparedCommand(UDialogueCommand* command, TArray<FString> args, AActor* targetActor) {
    this->arguments = args;
    this->targetActor = targetActor;
    this->logic = command;
}

FPreparedCommand::~FPreparedCommand() {
}

bool FPreparedCommand::Verify() const {
    // check target actor type being correct.
    const auto requiredClass = logic->TargetActorType();
    if (requiredClass && !targetActor->IsA(requiredClass)) {
        return false;
    }
    // check arguments
    return (arguments.Num() >= logic->MinArguments() || logic->MinArguments() < 0) && (arguments.Num() <= logic->MaxArguments() || logic->MaxArguments() < 0);
}

void FPreparedCommand::Run() const {
    logic->Execute(targetActor, arguments);
}
