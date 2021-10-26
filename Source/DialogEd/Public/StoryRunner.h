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
    UPROPERTY()
    APlayerController* instigatorController;
    
    UPROPERTY()
    ACharacter* instigatorCharacter;

protected:
    /** Action mapping to poll for when progressing the text or any such thing. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName inputAction;
    
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
    void StartNewStoryThread(UStoryThread* story, APlayerController* controller);

    UFUNCTION(BlueprintCallable)
    void StartThreadFromAsset(UStoryAsset* asset, FString threadName, APlayerController* controller);

    void SetMessageManager(FMessageManager* manager) {
        messageManager = manager;
    }


};
