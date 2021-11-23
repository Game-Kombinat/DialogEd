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
    FString rawValue = cmd->argumentArray[1];
    if (!UKismetStringLibrary::IsNumeric(rawValue)) {
        LOG_ERROR("Cannot set variable %s because the value %s is not numeric. Only numerics allowed!", *name, *rawValue);
    }

    int value = UKismetStringLibrary::Conv_StringToInt(rawValue);
    storyRunner->GetDataContext()->SetValue(name, value);
}

bool USetVarCommand::IsFinished() {
    return true;
}

void USetVarCommand::Cleanup() {
}
