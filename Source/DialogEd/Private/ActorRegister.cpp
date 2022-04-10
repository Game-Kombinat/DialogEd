// Copyright (c) Game Kombinat


#include "ActorRegister.h"

#include "ActorLookupRow.h"
#include "Logging.h"
#include "Kismet/GameplayStatics.h"

void UActorRegister::Init(UWorld* world) {
    resolvedActors.Empty();
    this->activeWorld = world;
}

UDialogueActor* UActorRegister::GetActorForTag(FName tag) {
    if (resolvedActors.Contains(tag)) {
        return resolvedActors[tag];
    }
   
    UGameplayStatics::GetAllActorsWithTag(activeWorld, tag, actorLookupMap);
    if(actorLookupMap.Num() == 0) {
        LOG_WARNING("No actors with tag %s exist in current world.", *tag.ToString())
        return nullptr;
    }
    for (int i = 0; i < actorLookupMap.Num(); ++i) {
        const auto actor  = actorLookupMap[i];
        UDialogueActor* diagActor = Cast<UDialogueActor>(actor->GetComponentByClass(UDialogueActor::StaticClass()));
        if (diagActor) {
            resolvedActors.Add(tag, diagActor);
            return diagActor;
        }
    }
    LOG_WARNING("There are actors with tag %s but none has a UDialogueActor component on them!", *tag.ToString())
    return nullptr;
}

UDialogueActor* UActorRegister::GetActorForTag(FString tag) {
    return GetActorForTag(FName(tag));
}
