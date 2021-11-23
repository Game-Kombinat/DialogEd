// Copyright (c) Game Kombinat


#include "PreparedCommand.h"

#include "DialogueActor.h"
#include "DialogueCommand.h"

FPreparedCommand::FPreparedCommand() : command(MakeShared<FParsedCommand>()) {
    logic = nullptr;
}

FPreparedCommand::FPreparedCommand(UDialogueCommand* cmd, TSharedRef<FParsedCommand> rawCmd) : command(rawCmd) {
    logic = cmd;
}

FPreparedCommand::~FPreparedCommand() {
}

bool FPreparedCommand::HasValidSetup() const {
    return logic != nullptr;
}

bool FPreparedCommand::Verify() const {
    if (!HasValidSetup()) {
        return false;
    }
    // check target actor type being correct.
    // const auto requiredClass = logic->TargetActorType();
    // if (requiredClass && !targetActor->GetOwner()->IsA(requiredClass)) {
    //     return false;
    // }

    // check arguments
    return (command->argumentArray.Num() >= logic->MinArguments() || logic->MinArguments() < 0) && (command->argumentArray.Num() <= logic->MaxArguments() || logic->MaxArguments() < 0);
}

bool FPreparedCommand::IsFinished() const {
    if (!logic) {
        return true;
    }
    return logic->IsFinished();
}

void FPreparedCommand::Run() const {
    logic->Execute(command);
}

void FPreparedCommand::Cleanup() {
    if (logic) {
        logic->Cleanup();
    }
    logic = nullptr;
}
