﻿#pragma once

#include "CoreMinimal.h"
#include "Logging.h"
#include "Kismet/KismetStringLibrary.h"
#include "ParsedCommand.generated.h"

/**
 * Raw parsed command line data.
 * THis is stored in a StoryThread.
 */
USTRUCT(BlueprintType)
struct FParsedCommand {
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString commandName;

    /** name of the target actor (as mapped in ActorNameMap data table) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString targetActor;

    /** List of arguments sans the actor name */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString argumentList;

    FParsedCommand() {
        // just to signify that this is wrong.
        commandName = "unknown";
        targetActor = "none";
    }

    FParsedCommand(const FString name, const FString args) : commandName(name) {
        int idx;
        if (args.FindChar(' ', idx)) {
            targetActor = UKismetStringLibrary::GetSubstring(args, 0, idx);
            argumentList = args.RightChop(idx);
        }
        else {
            LOG_ERROR("Failed to parse the text command %s with argument list %s", *name, *args);
        }
    }
};