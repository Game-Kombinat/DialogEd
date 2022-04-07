// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "DataContextContainer.h"
#include "GameDataContext.h"
#include "Components/ActorComponent.h"
#include "StoryRunner.generated.h"

class UStoryThread;
class UDialogueActor;

USTRUCT()
struct FDialogData {
    GENERATED_BODY()
    UPROPERTY()
    UDialogueActor* dialogueActor;
    
    UPROPERTY()
    FString message;

    TMap<int, FString> choices;
};

UENUM()
enum class ERunnerState : uint8 {
    // All is well, the dialog data can be used
    Ok,
    // When attempting to call Next() when we're at a choice node
    NeedNext,
    // dialog data contains choices
    Choices,
    // story is through.
    Done
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStoryFinishedCallback);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStoryStartedCallback);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIALOGED_API UStoryRunner : public UActorComponent, public IDataContextContainer {
    GENERATED_BODY()

    // The node that is the root of the current story.
    UPROPERTY(Transient)
    class UThreadNode* threadNode;

    // Currently processing base node.
    // Tree consists of a number of nodes connected by their right-side children,
    // Where the left side is the actual execution in that node.
    UPROPERTY(Transient)
    class UDialogNode* currentNode;

    // the left child of the currentNode.
    UPROPERTY(Transient)
    TArray<UDialogNode*> branchNodeStack;

    UPROPERTY()
    class UStoryAsset* storyAsset;
    
    UPROPERTY(EditAnywhere)
    UGameDataContext* dataContext;

protected:

    // todo: we probably gonna need those for evaluating the nodes. If the runner becomes a world subsystem, gonna find out how to get these in here.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class UDialogueCommandRegister* commandRegister;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class UActorRegister* actorRegister;

    

public:
    
    UPROPERTY()
    TArray<UDialogueActor*> actorsInActiveThread;
    // Sets default values for this component's properties
    UStoryRunner();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void HandleActorsInThread();

public:
    void CountRan(const UThreadNode* thread) const;

    UFUNCTION()
    virtual UGameDataContext* GetDataContext() override;

    bool HasNext();
    bool HandleIfElseBranching();
    bool HandleBranchingNodeTypes();
    void ShiftToNextNode();
    void GoToNextDialogNode();

    ERunnerState GetCurrent(FDialogData& dialogData);
    
    ERunnerState Next();

    ERunnerState NextWithChoice(int choice);

    UFUNCTION(BlueprintCallable, meta=(Latent))
    void StartThreadFromAsset(UStoryAsset* asset, FString threadName);

    void SetDataContext(UGameDataContext* dc) {
        dataContext = dc;
    }

    UDialogueActor* GetDialogueActor(const FString& nameOrTag) const;
};
