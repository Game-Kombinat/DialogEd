// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "PreparedCommand.h"
#include "StoryAsset.h"
#include "StoryThread.h"
#include "Components/ActorComponent.h"
#include "StoryRunner.generated.h"

/**
 * Runs a StoryThread until its done.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIALOGED_API UStoryRunner : public UActorComponent {
    GENERATED_BODY()

    UPROPERTY(Transient)
    UStoryThread* currentThread;
    
    FPreparedCommand currentCommand;

    /** Used so we don't expose widgets in non-ui specific code parts. */
    FMessageManager* messageManager;

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

    UFUNCTION(BlueprintCallable)
    void StartNewStoryThread(UStoryThread* story);

    UFUNCTION(BlueprintCallable)
    void StartThreadFromAsset(UStoryAsset* asset, FString threadName);

    void SetMessageManager(FMessageManager* manager) {
        messageManager = manager;
    }


};
