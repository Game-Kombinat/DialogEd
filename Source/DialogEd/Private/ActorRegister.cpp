// Copyright (c) Game Kombinat


#include "ActorRegister.h"

#include "ActorLookupRow.h"
#include "Logging.h"
#include "Kismet/GameplayStatics.h"

void UActorRegister::OnBeginPlay(UWorld* world) {
    resolvedActors.Empty();
    this->activeWorld = world;
}

UDialogueActor* UActorRegister::GetActorForTag(FName tag) {
    if (resolvedActors.Contains(tag)) {
        return resolvedActors[tag];
    }
    const FString contextString;
    const auto row = tagToTypeMap->FindRow<FActorLookupRow>(tag, contextString);
    if (!row) {
        LOG_WARNING("Looking for actor with tag %s - no type map found.", *(tag.ToString()));
        return nullptr;
    }

    const auto actor = UGameplayStatics::GetActorOfClass(activeWorld, row->type);
    if (!actor) {
        LOG_WARNING("Looking for actor with type %s - none found", *(row->type->GetName()));
        return nullptr;
    }
    
    UDialogueActor* diagActor = Cast<UDialogueActor>(actor->GetComponentByClass(UDialogueActor::StaticClass()));
    if (!diagActor) {
        LOG_ERROR("actor with tag %s - has no DialogueActor component on it! ");
        return nullptr;
    }
    
    resolvedActors.Add(tag, diagActor);
    return diagActor;
}

UDialogueActor* UActorRegister::GetActorForTag(FString tag) {
    return GetActorForTag(FName(tag));
}
