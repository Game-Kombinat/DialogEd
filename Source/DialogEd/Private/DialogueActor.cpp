// Copyright (c) Game Kombinat


#include "DialogueActor.h"


// Sets default values for this component's properties
UDialogueActor::UDialogueActor() {
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
}

FString UDialogueActor::GetActorName() const {
    return actorName;
}

FColor UDialogueActor::GetBaseTextColor() const {
    return baseTextColor;
}