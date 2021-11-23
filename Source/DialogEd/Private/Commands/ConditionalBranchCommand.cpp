// Copyright (c) Game Kombinat


#include "Commands/ConditionalBranchCommand.h"

#include "Logging.h"
#include "StoryRunner.h"

UConditionalBranchCommand::UConditionalBranchCommand() {
}

void UConditionalBranchCommand::Execute(TSharedRef<FParsedCommand> cmd) {
    auto strings = cmd->argumentArray;
    const auto dc = this->storyRunner->GetDataContext();
    FOperand lhs(strings[0], dc);
    FOperand rhs(strings[2], dc);
    const FString op = strings[1]; // this was sanitized while parsing
    // we can't switch on strings and I don't want to do any further processing here so lets go.
    bool result = false;
    if (op.Equals("==")) {
        result = lhs.GetValue() == rhs.GetValue();
    }
    else if (op.Equals("!=")) {
        result = lhs.GetValue() != rhs.GetValue();
    }
    else if (op.Equals(">")) {
        result = lhs.GetValue() > rhs.GetValue();
    }
    else if (op.Equals(">=")) {
        result = lhs.GetValue() >= rhs.GetValue();
    }
    else if (op.Equals("<")) {
        result = lhs.GetValue() < rhs.GetValue();
    }
    else if (op.Equals("<=")) {
        result = lhs.GetValue() <= rhs.GetValue();
    }

    // account for the fact that we may not have an else branch as it is optional
    if (!result && cmd->branches.Num() > 1) {
        myThread->SetBranchingTarget(1);
    }
    else if (result) {
        myThread->SetBranchingTarget(0);
    }
}

bool UConditionalBranchCommand::IsFinished() {
    return true;
}

void UConditionalBranchCommand::Cleanup() {
    
}
