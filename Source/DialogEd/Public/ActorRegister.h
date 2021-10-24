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
class DIALOGED_API UActorRegister : public UDataAsset {
    GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UDataTable* tagToTypeMap;

    UPROPERTY(VisibleAnywhere, Transient)
    TMap<FName, UDialogueActor*> resolvedActors;

    UPROPERTY(VisibleAnywhere, Transient)
    UWorld* activeWorld;

public:
    /** Mostly useful in editor because resolvedActors will not be wiped between PIE sessions. */
    void OnBeginPlay(UWorld* world);

    UDialogueActor* GetActorForTag(FName tag);
    UDialogueActor* GetActorForTag(FString tag);
};
