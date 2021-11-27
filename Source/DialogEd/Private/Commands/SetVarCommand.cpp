// Copyright (c) Game Kombinat


#include "Commands/SetVarCommand.h"

#include "Logging.h"
#include "StoryRunner.h"
#include "Kismet/KismetStringLibrary.h"


USetVarCommand::USetVarCommand() {
}

USetVarCommand::~USetVarCommand() {
}

void USetVarCommand::Execute(TSharedRef<FParsedCommand> cmd) {
    FString name = cmd->argumentArray[0];
    FString newKey;
    bool needForce = false;
    // keys starting with __ are auto-keys that don't exist in the design-time data context model.
    // They are local to each branch and are created dynamically on demand.
    if (name.StartsWith("__")) {
        newKey = FString::Format(TEXT("{0}{1}"), {myThread->GetStoryThreadName(), name});
        needForce = true;
    }
    else {
        newKey = name;
    }

    
    FString rawValue = cmd->argumentArray[1];
    if (!UKismetStringLibrary::IsNumeric(rawValue)) {
        LOG_ERROR("Cannot set variable %s because the value %s is not numeric. Only numerics allowed!", *newKey, *rawValue);
    }

    int value = UKismetStringLibrary::Conv_StringToInt(rawValue);
    if (needForce) {
        storyRunner->GetDataContext()->ForceSetValue(newKey, value);
    }
    else {
        storyRunner->GetDataContext()->SetValue(newKey, value);
    }
    
}

bool USetVarCommand::IsFinished() {
    return true;
}

void USetVarCommand::Cleanup() {
}
