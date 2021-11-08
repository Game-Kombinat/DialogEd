#pragma once

#include "CoreMinimal.h"
#include "DialogueBranchId.h"
#include "ParsedCommand.generated.h"

/**
 * Raw parsed command line data.
 * THis is stored in a StoryThread.
 */
USTRUCT(BlueprintType)
struct FParsedCommand {
    GENERATED_BODY()

    static int parsingIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString commandName;

    /** name of the target actor (as mapped in ActorNameMap data table) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString targetActor;

    /** If this command required the targetActor to be resolved or not. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool requiresActor;

    /** List of arguments sans the actor name */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString argumentList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDialogueBranchId> branches;

    /** This ID is required to create actually unique branch IDs */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int parsingId;

    FParsedCommand() {
        // just to signify that this is wrong.
        commandName = "unknown";
        targetActor = "none";
        requiresActor = true;
        parsingId = -1;
    }

    FParsedCommand(const FString name, const FString actorName, const FString args) : commandName(name), targetActor(actorName), requiresActor(true), argumentList(args) {
        parsingId = parsingIndex++;
    }
    FParsedCommand(const FString name, const FString args) : commandName(name), requiresActor(false), argumentList(args) {
        parsingId = parsingIndex++;
    }

    int AddBranch(const FString branchTitle) {

        return branches.Add(FDialogueBranchId(
                branchTitle,
                FString::Format(TEXT("{0}_{1}_{2}"), {
                        parsingId,
                        commandName,
                        branchTitle
                    }
                )
            )
        );
    }

    /** Returns the branch display title (aka the question) */
    FString GetBranchTitle(const int idx) const {
        return branches[idx].title;
    }

    /** Returns the branch id (address to a sub thread within the thread) */
    FString GetBranchId(const int idx) const {
        return branches[idx].branchId;
    }

    int NumBranches() const {
        return branches.Num();
    }

    /** Call before parsing a new story file */
    static void ResetParsingIndex() {
        parsingIndex = 0;
    }
};

int FParsedCommand::parsingIndex = 0;