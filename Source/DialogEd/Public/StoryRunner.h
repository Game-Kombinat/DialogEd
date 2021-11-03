// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
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

    // UPROPERTY(Transient)
    // UStoryThread* currentThread;
    
    UPROPERTY(Transient)
    TArray<UStoryThread*> threadStack;
    
    /** Used so we don't expose widgets in non-ui specific code parts. */
    UPROPERTY()
    class UMessageManager* messageManager;
    UPROPERTY()
    APlayerController* instigatorController;
    
    UPROPERTY()
    ACharacter* instigatorCharacter;

    UPROPERTY()
    UStoryAsset* storyAsset;

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
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
    void StartNewStoryThread(UStoryThread* thread, APlayerController* controller);

    UFUNCTION(BlueprintCallable)
    void StartThreadFromAsset(UStoryAsset* asset, FString threadName, APlayerController* controller);

    void SetMessageManager(UMessageManager* manager) {
        messageManager = manager;
    }
};
