// Copyright (c) Game Kombinat


#include "Commands/ConditionalBranchCommand.h"

#include "StoryRunner.h"

UConditionalBranchCommand::UConditionalBranchCommand() {
}

void UConditionalBranchCommand::Execute(TSharedRef<FParsedCommand> cmd) {
    auto strings = cmd->argumentArray;
    const auto dc = this->storyRunner->GetDataContext();
    // todo: we could replace the interpretation of arguments bit with something
    // more flexible like a pool of interpreters to allow for different kinds of branching contexts etc.
    
    // inputs were all sanitized when the data was parsed from text file.
    // See FileParser class
    FOperand lhs = MakeOperand(strings[0], dc);
    FOperand rhs = MakeOperand(strings[2], dc);
    const FString op = strings[1];
    
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

FOperand UConditionalBranchCommand::MakeOperand(const FString& keyName, UGameDataContext* dc) const {
    FString newKey;
    // keys starting with __ are auto-keys that don't exist in the design-time data context model.
    // They are local to each branch and are created dynamically on demand.
    if (keyName.StartsWith("__")) {
        newKey = FString::Format(TEXT("{0}{1}"), {myThread->GetStoryThreadName(), keyName});
    }
    else {
        newKey = keyName;
    }

    return FOperand(newKey, dc);
}
