// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "DialogueActor.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIALOGED_API UDialogueActor : public USceneComponent {
    GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dialogue Data")
    FString actorName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dialogue Data")
    FColor baseTextColor;
public:
    // Sets default values for this component's properties
    UDialogueActor();
    FString GetActorName() const;
    FColor GetBaseTextColor() const;
};
