#pragma once

#include "CoreMinimal.h"
#include "DialogueBranchId.h"
#include "Logging.h"
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


    /** List of arguments sans the actor name */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString argumentList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> argumentArray;

    /** Arguments as string with the first word removed */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString trimmedArgumentList;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDialogueBranchId> branches;

    /** This ID is required to create actually unique branch IDs */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int parsingId;
    

    FParsedCommand() {
        // just to signify that this is wrong.
        commandName = "unknown";
        targetActor = "none";
        parsingId = -1;
    }
    
    FParsedCommand(const FString name, const FString args) : commandName(name), argumentList(args) {
        parsingId = parsingIndex++;
        args.ParseIntoArray(argumentArray, TEXT(" "));

        if (argumentArray.Num() > 0) {
            // If a command doesn't specify actor as first argument
            // because it doesn't need it or expects it at a different spot, this will be wrong.
            // But it's for convenience for commands that do it this way (the right way)
            targetActor = argumentArray[0];
            // There is probably a more elegant way to create a string with first word removed but eh.
            // This is at editor time so whatever.
            TArray<FString> tmp(argumentArray);
            tmp.RemoveAt(0);
            trimmedArgumentList = FString::Join(tmp, TEXT(" "));
        }
        
        
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

    FString MakeThreadName(const FString& outerName, const FString& threadName) const {
        return FString::Format(TEXT("({0})->{1}[{2}]->({3})"), { outerName, commandName, branches.Num(), threadName});
    }

    /** Call before parsing a new story file */
    static void ResetParsingIndex() {
        parsingIndex = 0;
    }
};

int FParsedCommand::parsingIndex = 0;
