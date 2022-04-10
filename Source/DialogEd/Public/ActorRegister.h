// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "DialogueActor.h"
#include "Engine/DataTable.h"
#include "UObject/Object.h"
#include "ActorRegister.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGED_API UActorRegister : public UObject {
    GENERATED_BODY()
protected:

    UPROPERTY(VisibleAnywhere, Transient)
    TMap<FName, UDialogueActor*> resolvedActors;

    UPROPERTY(VisibleAnywhere, Transient)
    UWorld* activeWorld;

    UPROPERTY()
    TArray<AActor*> actorLookupMap; // used for resolving actors.

public:
    /** Mostly useful in editor because resolvedActors will not be wiped between PIE sessions. */
    void Init(UWorld* world);

    UDialogueActor* GetActorForTag(FName tag);
    UDialogueActor* GetActorForTag(FString tag);
};
