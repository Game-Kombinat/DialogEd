// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "DataContextContainer.h"
#include "GameDataContext.h"
#include "Components/ActorComponent.h"
#include "StoryRunner.generated.h"

class UStoryThread;
class UDialogueActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStoryFinishedCallback);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStoryStartedCallback);
/**
 * Runs a StoryThread until its done.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIALOGED_API UStoryRunner : public UActorComponent, public IDataContextContainer {
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
    class UStoryAsset* storyAsset;
    
    UPROPERTY()
    UGameDataContext* dataContext;

protected:
    /** Action mapping to poll for when progressing the text or any such thing. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName inputAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class UDialogueCommandRegister* commandRegister;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class UActorRegister* actorRegister;

    

public:
    UPROPERTY(BlueprintAssignable)
    FStoryFinishedCallback onFinished;
    
    UPROPERTY(BlueprintAssignable)
    FStoryStartedCallback onStarted;
    
    UPROPERTY()
    TArray<UDialogueActor*> actorsInActiveThread;
    // Sets default values for this component's properties
    UStoryRunner();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void HandleActorsInThread();

public:
    void Observe(UObject* obj, ELatentActionChangeType changeType);
    void CountRan(const UStoryThread* thread) const;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION()
    virtual UGameDataContext* GetDataContext() override; 

    void StartNewStoryThread(UStoryThread* thread, APlayerController* controller);

    UFUNCTION(BlueprintCallable, meta=(Latent))
    void StartThreadFromAsset(UStoryAsset* asset, FString threadName, APlayerController* controller);

    void SetMessageManager(UMessageManager* manager) {
        messageManager = manager;
    }

    void SetDataContext(UGameDataContext* dc) {
        dataContext = dc;
    }

    UFUNCTION(BlueprintCallable)
    bool IsRunning() const;

    UMessageManager* GetMessageManager() const;
    UDialogueActor* GetDialogueActor(const FString& nameOrTag) const;
};
