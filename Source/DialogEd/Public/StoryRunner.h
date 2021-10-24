// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "PreparedCommand.h"
#include "StoryThread.h"
#include "Components/ActorComponent.h"
#include "StoryRunner.generated.h"

/**
 * Runs a StoryThread until its done.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIALOGED_API UStoryRunner : public UActorComponent {
    GENERATED_BODY()
    
    FStoryThread* currentThread;
    
    FPreparedCommand currentCommand;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class UDialogueCommandRegister* commandRegister;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class UActorRegister* actorRegister;

public:
    // Sets default values for this component's properties
    UStoryRunner();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void StartNewStoryThread(FStoryThread* story);


};
