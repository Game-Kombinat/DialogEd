// Copyright (c) Game Kombinat


#include "PreparedCommand.h"

#include "DialogueActor.h"
#include "DialogueCommand.h"

FPreparedCommand::FPreparedCommand() {
    logic = nullptr;
    command = nullptr;
}

FPreparedCommand::FPreparedCommand(TWeakObjectPtr<class UDialogueCommand> cmd, TSharedPtr<FParsedCommand> rawCmd) : logic(cmd), command(rawCmd) {
}

FPreparedCommand::~FPreparedCommand() {
}

bool FPreparedCommand::HasValidSetup() const {
    return logic.IsValid() && command.IsValid();
    // return IsValid(logic);
}

bool FPreparedCommand::Verify() const {
    if (!HasValidSetup()) {
        return false;
    }

    return (command->argumentArray.Num() >= logic->MinArguments() || logic->MinArguments() < 0) && (command->argumentArray.Num() <= logic->MaxArguments() || logic->MaxArguments() < 0);
}

bool FPreparedCommand::IsFinished() const {
    if (!logic.IsValid()) {
        return true;
    }
    return logic->IsFinished();
}

void FPreparedCommand::Run() const {
    logic->Execute(command);
}

void FPreparedCommand::Cleanup() {
    if (logic.IsValid()) {
        logic->Cleanup();
    }
    logic.Reset();
}
